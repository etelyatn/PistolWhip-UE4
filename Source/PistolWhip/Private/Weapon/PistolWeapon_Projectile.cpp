// 2021 github.com/EugeneTel/PistolWhip-UE4


#include "Weapon/PistolWeapon_Projectile.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/PistolProjectile.h"

void APistolWeapon_Projectile::FireWeapon()
{
	const FVector ProjectileStart = GetMuzzleLocation();

	const FVector ProjectileGoal = GetProjectileGoal();
	const FRotator ProjectileDir = UKismetMathLibrary::FindLookAtRotation(ProjectileStart, ProjectileGoal);
	
	FireProjectile(ProjectileStart, ProjectileDir);
}

FVector APistolWeapon_Projectile::GetProjectileGoal()
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
	const FTransform SpawnTransform(ProjectileDir, Origin);
	APistolProjectile* Projectile = Cast<APistolProjectile>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ProjectileConfig.ProjectileClass, SpawnTransform));
	if (Projectile)
	{
		Projectile->SetInstigator(GetInstigator());
		Projectile->SetOwner(this);

		UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);
	}
}
