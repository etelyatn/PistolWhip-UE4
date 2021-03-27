// 2021 github.com/EugeneTel/PistolWhip-UE4


#include "Player/Components/PistolPlayerHealthComponent.h"
#include "PistolWhip.h"
#include "Components/PostProcessComponent.h"
#include "Enemy/PistolEnemyPawn.h"
#include "Player/PistolPlayerPawn.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Player/PistolPlayerState.h"

// Sets default values for this component's properties
UPistolPlayerHealthComponent::UPistolPlayerHealthComponent()
{

}

void UPistolPlayerHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerPawn)
	{
		// subscribe on events
		const AController* PC = PlayerPawn->GetController();
		APistolPlayerState* PS = PC ? PC->GetPlayerState<APistolPlayerState>() : nullptr;
		if (PS)
		{
			PS->OnEnemyHit.AddUObject(this, &UPistolPlayerHealthComponent::OnEnemyHit);
		}
	}
}

void UPistolPlayerHealthComponent::SetPlayerPawn(APistolPlayerPawn* InPawn)
{
	PlayerPawn = InPawn;
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
		OnShieldDestroyed.Broadcast(ShieldData.RestoreHits);
	}
	
	NotifyShieldDestroyed();
}

void UPistolPlayerHealthComponent::ShieldFullyRestored()
{
	ShieldData.ResetShield();

	if (OnShieldFullyRestored.IsBound())
	{
		OnShieldFullyRestored.Broadcast();
	}

	NotifyShieldFullyRestored();
}

void UPistolPlayerHealthComponent::RestoreShield(const int Hits)
{
	if (HealthData.bAlive && ShieldData.bActive && ShieldData.bDestroyed)
	{
		ShieldData.CurrentHits += Hits;

		if (OnShieldRestoreProgress.IsBound())
		{
			OnShieldDestroyed.Broadcast(ShieldData.CurrentHits);
		}

		if (ShieldData.CurrentHits >= ShieldData.RestoreHits)
		{
			ShieldFullyRestored();
		}
	}
}

void UPistolPlayerHealthComponent::OnEnemyHit(APistolEnemyPawn* EnemyPawn)
{
	// try to restore the shield
	RestoreShield(1);
}


