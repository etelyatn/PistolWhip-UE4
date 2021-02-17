// 2021 github.com/EugeneTel/PistolWhip-UE4


#include "Player/Components/PistolPlayerHealthComponent.h"

#include "Log.h"
#include "PistolWhip.h"
#include "Components/PostProcessComponent.h"
#include "Enemy/PistolEnemyPawn.h"
#include "Player/PistolPlayerPawn.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values for this component's properties
UPistolPlayerHealthComponent::UPistolPlayerHealthComponent()
{

}

void UPistolPlayerHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	APistolEnemyPawn::OnHit.AddUObject(this, &UPistolPlayerHealthComponent::OnEnemyPawnHit);
}

void UPistolPlayerHealthComponent::TakeDamage(const float Amount)
{
	if (!HealthData.bAlive)
	{
		return;
	}

	// notify blueprints to show VFX and SFX
	NotifyTakeDamage();
	
	if (!HealthData.bImmortal)
	{
		// the shield takes damage first
		if (ShieldData.bActive && ShieldData.CurrentShield > 0.0f)
		{
			ShieldData.CurrentShield -= Amount;

			if (ShieldData.CurrentShield <= 0.0f)
			{
				DestroyShield();
			}
		}
		else
		{
			HealthData.CurrentHealth -= Amount;
		}

		if (HealthData.CurrentHealth <= 0.0f)
		{
			Death();
		}
	}
}

bool UPistolPlayerHealthComponent::IsAlive() const
{
	return HealthData.bAlive;
}

void UPistolPlayerHealthComponent::SetHealthData(FPlayerHealthData& InHealthData)
{
	HealthData = InHealthData;

	CreateDamageOverlay();
	SetDamageOverlayVisibility(0.0f);
	HealthData.ResetHealth();
}

void UPistolPlayerHealthComponent::SetShieldData(FPlayerShieldData& InShieldData)
{
	ShieldData = InShieldData;
	ShieldData.ResetShield();
}

void UPistolPlayerHealthComponent::Death()
{
	HealthData.bAlive = false;

	OnPlayerDeath.Broadcast();
	NotifyDeath();
}

void UPistolPlayerHealthComponent::DestroyShield()
{
	ShieldData.bDestroyed = true;
	
	if (OnShieldDestroyed.IsBound())
	{
		OnShieldDestroyed.Broadcast();
	}
	
	NotifyShieldDestroyed();
}

void UPistolPlayerHealthComponent::ShieldFullyRestored()
{
	ShieldData.ResetShield();

	if (OnShieldRestored.IsBound())
	{
		OnShieldRestored.Broadcast();
	}

	NotifyShieldFullyRestored();
}

void UPistolPlayerHealthComponent::RestoreShield(const int Hits)
{
	if (HealthData.bAlive && ShieldData.bActive && ShieldData.bDestroyed)
	{
		ShieldData.CurrentHits += Hits;

		if (ShieldData.CurrentHits >= ShieldData.RestoreHits)
		{
			ShieldFullyRestored();
		}
	}
}

void UPistolPlayerHealthComponent::CreateDamageOverlay()
{
	// create damage overlay
	if (HealthData.DamageOverlayMaterial && PlayerPawn)
	{
		HealthData.DamageOverlayMaterialInstance = UMaterialInstanceDynamic::Create(HealthData.DamageOverlayMaterial, this); 
		PlayerPawn->GetPostProcessComponent()->AddOrUpdateBlendable(HealthData.DamageOverlayMaterialInstance);
	}
}

void UPistolPlayerHealthComponent::SetDamageOverlayVisibility(const float Value)
{
	if (HealthData.DamageOverlayMaterialInstance)
	{
		HealthData.DamageOverlayMaterialInstance->SetScalarParameterValue(HealthData.DamageOverlayParameter, Value);
	}
	else
	{
		UE_LOG(LogPistolWhip, Log, TEXT("UPistolPlayerHealthComponent::SetDamageOverlayVisibility - DamageOverlayMaterial is not set!"));
	}
}

void UPistolPlayerHealthComponent::OnEnemyPawnHit(APistolEnemyPawn* EnemyPawn)
{
	// try to restore the shield
	RestoreShield(1);
}


