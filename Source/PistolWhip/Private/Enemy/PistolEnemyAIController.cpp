// 2021 github.com/EugeneTel/PistolWhip-UE4

#include "Enemy/PistolEnemyAIController.h"

#include "Log.h"
#include "Enemy/PistolEnemyPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/PistolPlayerPawn.h"
#include "Weapon/PistolWeapon_Projectile.h"

class APistolPlayerPawn;

void APistolEnemyAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	CachedEnemyPawn = Cast<APistolEnemyPawn>(InPawn);
}

void APistolEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_Firing, this, &APistolEnemyAIController::Fire, FiringDelay, true);

	CachedPlayerPawn = Cast<APistolPlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

void APistolEnemyAIController::UpdateControlRotation(float DeltaTime, bool bUpdatePawn)
{
	if (!CachedPlayerPawn.IsValid() || !CachedEnemyPawn.IsValid())
	{
		return;
	}
	
	const FRotator NewControlRotation = UKismetMathLibrary::FindLookAtRotation(CachedEnemyPawn->GetActorLocation(), CachedPlayerPawn->GetActorLocation());
	SetControlRotation(NewControlRotation);

	if (bUpdatePawn)
	{
		CachedEnemyPawn->FaceRotation(NewControlRotation);
	}	
}

void APistolEnemyAIController::Fire()
{
	if (!CachedEnemyPawn.IsValid() || !CachedPlayerPawn.IsValid())
	{
		return;
	}

	APistolWeapon_Projectile* Weapon = Cast<APistolWeapon_Projectile>(CachedEnemyPawn->GetWeapon());
	if (Weapon && CachedEnemyPawn->IsAlive())
	{
		/** Predict the player's head position when a projectile reach the head */
		float GoalReachTime = Weapon->GetGoalReachTime();
		FVector HeadLocation = CachedPlayerPawn->GetHeadLocation();
		const FVector MuzzleLocation = Weapon->GetMuzzleLocation();
		const FVector PlayerForwardVector = CachedPlayerPawn->GetActorForwardVector();

		const float PlayerAxisDistance = abs((PlayerForwardVector * HeadLocation).Size());
		const float EnemyAxisDistance = abs((PlayerForwardVector * MuzzleLocation).Size());

		// max hit distance is a half of the axis way
		const float MaxHitDistance = (EnemyAxisDistance - PlayerAxisDistance) / 2;
		float HitDistance = GoalReachTime * CachedPlayerPawn->GetMovementSpeed();

		// adjust goal reach time and hit distance to max hit distance
		if (MaxHitDistance < HitDistance)
		{
			HitDistance = MaxHitDistance;
			GoalReachTime = HitDistance / CachedPlayerPawn->GetMovementSpeed();
		}

		// do not shoot in the back of the player. only if enemy in forward
		if (PlayerAxisDistance + HitDistance < EnemyAxisDistance)
		{
			HeadLocation = HeadLocation + (PlayerForwardVector * HitDistance);
			const float ProjectileDistance = FVector::Dist(HeadLocation, MuzzleLocation);

			// do not shoot if very close to player
			if (HitDistance < ProjectileDistance)
			{
				const float ProjectileSpeed = ProjectileDistance / GoalReachTime;
				Weapon->SetProjectileSpeed(ProjectileSpeed);
				Weapon->SetGoalLocation(HeadLocation);
				Weapon->StartFire();
			}
		}
	}
}
