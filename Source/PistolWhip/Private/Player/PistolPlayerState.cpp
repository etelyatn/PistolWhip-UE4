// 2021 github.com/EugeneTel/PistolWhip-UE4


#include "Player/PistolPlayerState.h"
#include "Enemy/PistolEnemyPawn.h"
#include "Framework/PistolFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/PistolPlayerPawn.h"
#include "UI/Widgets/PistolPlayerInterfaceWidget.h"
#include "Weapon/PistolWeapon_Instant.h"

APistolPlayerState::APistolPlayerState()
{

}

void APistolPlayerState::NotifyPlayerDamage(float Damage, AController* EventInstigator, AActor* DamageCauser)
{
}

void APistolPlayerState::NotifyPlayerDeath()
{
}

void APistolPlayerState::NotifyEnemyHit(FEnemyHit& EnemyHit)
{
	OnEnemyHit.Broadcast(EnemyHit.EnemyPawn);
	
	CalculateEnemyDamageScore(EnemyHit);
}

void APistolPlayerState::CalculateEnemyDamageScore(FEnemyHit& EnemyHit)
{
	FVector AimDir = EnemyHit.ImpactPoint - EnemyHit.ShootStart;
	AimDir.Normalize();
				
	const float Angle = UPistolFunctionLibrary::AngleBetweenTwoVectorsInDegrees(EnemyHit.ShootDir, AimDir);

	const FIntPoint ScoreRange = EnemyHit.EnemyPawn ? EnemyHit.EnemyPawn->GetEnemyConfig().ScoreRange : FIntPoint();
	int32 DamageScore = ScoreRange.Y;

	// if hit using aiming (available only for instant weapon)
	APistolWeapon_Instant* Weapon = EnemyHit.Weapon ? Cast<APistolWeapon_Instant>(EnemyHit.Weapon) : nullptr;
	if (Angle > 0.1 && Weapon)
	{
		// calculate ratio depending from the aim radius and player accuracy 
		const float ScorePercent = UKismetMathLibrary::NormalizeToRange(Angle,0.0f, Weapon->GetAimRadius() / 10.0f);
		DamageScore = FMath::Max(ScoreRange.X, FMath::RoundToInt(DamageScore - DamageScore * ScorePercent));
	}

	TotalScore += DamageScore;

	// update player UI
	APistolPlayerPawn* PlayerPawn = GetPawn<APistolPlayerPawn>();
	if (PlayerPawn)
	{
		UPistolPlayerInterfaceWidget* UI = PlayerPawn->GetInterfaceWidget();
		if (UI)
		{
			UI->UpdateScore(TotalScore);
		}
	}

	// notify enemy pawn for score widget display
	if (EnemyHit.EnemyPawn)
	{
		EnemyHit.EnemyPawn->NotifyEnemyHit(DamageScore);
	}


}

