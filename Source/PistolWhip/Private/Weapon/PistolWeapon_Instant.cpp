// 2021 github.com/EugeneTel/PistolWhip-UE4


#include "Weapon/PistolWeapon_Instant.h"
#include "Enemy/PistolEnemyPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

void APistolWeapon_Instant::FireWeapon()
{
	const FVector StartTrace = GetMuzzleLocation();
	const FVector ShootDir = GetMuzzleDirection();
	const FVector EndTrace = StartTrace + ShootDir * WeaponRange;

	const FHitResult Impact = WeaponTrace(StartTrace, EndTrace);
	ProcessInstantHit(Impact, StartTrace, EndTrace, ShootDir);
}

void APistolWeapon_Instant::ProcessInstantHit(const FHitResult& Impact, const FVector& ShootStart,  const FVector& ShootEnd, const FVector& ShootDir)
{
	if (ShouldDealDamage(Impact.GetActor()))
	{
		DealDamage(Impact, ShootDir);
	}

	// FX
	const FVector EndPoint = Impact.GetActor() ? Impact.ImpactPoint : ShootEnd;
	SpawnTrailEffect(EndPoint);
}

bool APistolWeapon_Instant::ShouldDealDamage(AActor* TestActor) const
{
	// TODO: Implement complex checking
	
	return IsValid(TestActor);
}

void APistolWeapon_Instant::DealDamage(const FHitResult& Impact, const FVector& ShootDir)
{
	FPointDamageEvent PointDmg;
	PointDmg.DamageTypeClass = UDamageType::StaticClass();
	PointDmg.HitInfo = Impact;
	PointDmg.ShotDirection = ShootDir;
	PointDmg.Damage = HitDamage;

	if (IsValid(OwnerPawn) && IsValid(OwnerPawn->GetController()))
	{
		Impact.GetActor()->TakeDamage(PointDmg.Damage, PointDmg, OwnerPawn->GetController(), OwnerPawn);
		
		APistolEnemyPawn* EnemyPawn = Cast<APistolEnemyPawn>(Impact.GetActor());
		if (EnemyPawn)
		{
			EnemyPawn->GetMesh()->AddImpulse(ShootDir * HitMagnitude, Impact.BoneName);
		} else if (Impact.Component.IsValid() && Impact.Component->IsSimulatingPhysics())
		{
			Impact.Component->AddImpulseAtLocation(ShootDir * HitMagnitude, Impact.ImpactPoint);
		}
	}
}

void APistolWeapon_Instant::SpawnTrailEffect(const FVector& EndPoint) const
{
	if (!TrailFX)
		return;
    
	const FVector Origin = GetMuzzleLocation();

	UParticleSystemComponent* TrailPSC = UGameplayStatics::SpawnEmitterAtLocation(this, TrailFX, Origin, FRotator::ZeroRotator, FVector(1.0f));
	if (TrailPSC)
	{
		TrailPSC->SetVectorParameter(TrailTargetParam, EndPoint);
	}
}
