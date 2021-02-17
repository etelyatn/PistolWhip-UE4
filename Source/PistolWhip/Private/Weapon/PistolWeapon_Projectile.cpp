// 2021 github.com/EugeneTel/PistolWhip-UE4


#include "Weapon/PistolWeapon_Projectile.h"

#include "Log.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/PistolProjectile.h"

APistolWeapon_Projectile::APistolWeapon_Projectile()
{
	GoalLocation = FVector::ZeroVector;
}

float APistolWeapon_Projectile::GetProjectileSpeed() const
{
	return ProjectileConfig.ProjectileSpeed;
}

void APistolWeapon_Projectile::SetProjectileSpeed(const float InSpeed)
{
	ProjectileConfig.ProjectileSpeed = InSpeed;
}

float APistolWeapon_Projectile::GetGoalReachTime() const
{
	return ProjectileConfig.GoalReachTime;
}

void APistolWeapon_Projectile::FireWeapon()
{
	const FVector ProjectileStart = GetMuzzleLocation();
	const FVector ProjectileGoal = GetGoalLocation().IsZero() ? CalculateProjectileGoal() : GetGoalLocation();
	const FRotator ProjectileDir = UKismetMathLibrary::FindLookAtRotation(ProjectileStart, ProjectileGoal);
	
	FireProjectile(ProjectileStart, ProjectileDir);
}

FVector APistolWeapon_Projectile::CalculateProjectileGoal()
{
	const FVector StartTrace = GetMuzzleLocation();
	const FVector ShootDir = GetMuzzleDirection();
	
	const FVector EndTrace = StartTrace + ShootDir * WeaponRange;

	const FHitResult Impact = WeaponTrace(StartTrace, EndTrace);
	
	const FVector EndPoint = Impact.bBlockingHit ? Impact.ImpactPoint : Impact.TraceEnd;

	return EndPoint;
}

void APistolWeapon_Projectile::FireProjectile(const FVector Origin, const FRotator ProjectileDir)
{
	if (ProjectileConfig.ProjectileClass)
	{
		const FTransform SpawnTransform(ProjectileDir, Origin);
		APistolProjectile* Projectile = Cast<APistolProjectile>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ProjectileConfig.ProjectileClass, SpawnTransform));
		if (Projectile)
		{
			Projectile->SetInstigator(GetInstigator());
			Projectile->SetOwner(this);
			Projectile->InitProjectileSpeed(ProjectileConfig.ProjectileSpeed);
			// Projectile->InitVelocity()

			UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);
		}
	}
}
