// 2021 github.com/EugeneTel/PistolWhip-UE4

#include "Enemy/PistolEnemyAIController.h"
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
	if (CachedEnemyPawn.IsValid())
	{
		// subscribe on delegates
		CachedEnemyPawn->OnPawnMovementFinished.AddUObject(this, &APistolEnemyAIController::OnMovementFinished);
		CachedEnemyPawn->OnWeaponEquipped.AddUObject(this, &APistolEnemyAIController::OnWeaponEquipped);
	}
}

void APistolEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	CachedPlayerPawn = Cast<APistolPlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

void APistolEnemyAIController::UpdateControlRotation(float DeltaTime, bool bUpdatePawn)
{
	if (!CachedPlayerPawn.IsValid() || !CachedEnemyPawn.IsValid())
	{
		return;
	}

	FRotator NewControlRotation;
	if (CachedEnemyPawn->GetMovementType() == EPawnMovementType::PMT_Idle)
	{
		// look at the player
		NewControlRotation = UKismetMathLibrary::FindLookAtRotation(CachedEnemyPawn->GetActorLocation(), CachedPlayerPawn->GetActorLocation());
	}
	else
	{
		NewControlRotation = CachedEnemyPawn->GetCurrentSplineRotation();
	}

	SetControlRotation(NewControlRotation);

	if (bUpdatePawn)
	{
		CachedEnemyPawn->FaceRotation(NewControlRotation);
	}	
}

void APistolEnemyAIController::InitFiring()
{
	if (CachedEnemyPawn.IsValid() && CachedEnemyPawn->IsFiringEnabled())
	{
		const float FiringDelay = FMath::RandRange(CachedEnemyPawn->GetEnemyConfig().FirstShotDelayRange.X, CachedEnemyPawn->GetEnemyConfig().FirstShotDelayRange.X);
		GetWorldTimerManager().SetTimer(TimerHandle_FirstShot, this, &APistolEnemyAIController::Fire, FiringDelay, false);
	}
}

void APistolEnemyAIController::Fire()
{
	if (!CachedEnemyPawn.IsValid() || !CachedPlayerPawn.IsValid())
	{
		return;
	}

	APistolWeapon_Projectile* Weapon = Cast<APistolWeapon_Projectile>(CachedEnemyPawn->GetWeapon());
	if (Weapon && CachedEnemyPawn->IsAlive() && CachedEnemyPawn->GetEnemyConfig().bFiringEnabled)
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

		// set other shots
		const float FiringDelay = FMath::RandRange(CachedEnemyPawn->GetEnemyConfig().OtherShotsDelayRange.X, CachedEnemyPawn->GetEnemyConfig().OtherShotsDelayRange.X);
		GetWorldTimerManager().SetTimer(TimerHandle_OtherShots, this, &APistolEnemyAIController::Fire, FiringDelay, true);
	}
}

void APistolEnemyAIController::OnMovementFinished()
{
	// init the enemy firing timer
	InitFiring();
}

void APistolEnemyAIController::OnWeaponEquipped()
{
	// init firing from start for not movable enemies
	if (CachedEnemyPawn.IsValid() && CachedEnemyPawn->GetMovementType() == EPawnMovementType::PMT_Idle && CachedEnemyPawn->IsFiringEnabled())
	{
		InitFiring();
	}
}
