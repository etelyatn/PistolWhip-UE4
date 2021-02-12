// 2021 github.com/EugeneTel/PistolWhip-UE4


#include "Player/Components/PistolPlayerHealthComponent.h"

#include "Log.h"
#include "PistolWhip.h"
#include "Components/PostProcessComponent.h"
#include "Player/PistolPlayerPawn.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values for this component's properties
UPistolPlayerHealthComponent::UPistolPlayerHealthComponent()
{

}

void UPistolPlayerHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPistolPlayerHealthComponent::TakeDamage(const float Amount)
{
	if (!HealthData.bImmortal)
	{
		HealthData.CurrentHealth -= Amount;
	}

	NotifyTakeDamage();
	
	if (HealthData.CurrentHealth <= 0.0f && !HealthData.bImmortal)
	{
		Death();
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

void UPistolPlayerHealthComponent::Death()
{
	HealthData.bAlive = false;

	OnPlayerDeath.Broadcast();
	NotifyDeath();
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


