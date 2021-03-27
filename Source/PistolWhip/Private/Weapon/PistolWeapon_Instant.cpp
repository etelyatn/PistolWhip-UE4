// 2021 github.com/EugeneTel/PistolWhip-UE4


#include "Weapon/PistolWeapon_Instant.h"
#include "Enemy/PistolEnemyPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "PistolWhipTypes.h"

APistolWeapon_Instant::APistolWeapon_Instant()
{
	// defaults
	bUseAim = true;
	HitDamage = 1;
	HitMagnitude = 10000.0f;
	AimRadius = 100.f;
}

void APistolWeapon_Instant::FireWeapon()
{
	const FVector StartTrace = GetDamageStartLocation();
	const FVector ShootDir = GetAdjustedAim();
	const FVector EndTrace = StartTrace + ShootDir * WeaponRange;

	const FHitResult Impact = WeaponTrace(StartTrace, EndTrace);

	// use aiming if the hit 
	if (bUseAim && (!Impact.GetActor() || !Impact.GetActor()->IsA(APistolEnemyPawn::StaticClass())))
	{
		const FHitResult ImpactAim = AimTrace(StartTrace, EndTrace);

		// adjust hit to the aiming helper result
		if (ImpactAim.bBlockingHit)
		{
			ProcessInstantHit(ImpactAim, StartTrace, ImpactAim.ImpactPoint, ShootDir);
			return;
		}
	}
	
	ProcessInstantHit(Impact, StartTrace, EndTrace, ShootDir);
}

void APistolWeapon_Instant::ProcessInstantHit(const FHitResult& Impact, const FVector& ShootStart,  const FVector& ShootEnd, const FVector& ShootDir)
{
	if (ShouldDealDamage(Impact.GetActor()))
	{
		DealDamage(Impact, ShootDir);

		// notify player state
		APistolPlayerState* const PS = EquippingPawn ? Cast<APistolPlayerState>(EquippingPawn->GetPlayerState()) : nullptr;
		APistolEnemyPawn* EnemyPawn = Cast<APistolEnemyPawn>(Impact.GetActor());
		if (EnemyPawn && PS)
		{
			FEnemyHit EnemyHit;
			EnemyHit.bBlockingHit = Impact.bBlockingHit;
			EnemyHit.EnemyPawn = EnemyPawn;
			EnemyHit.ImpactPoint = Impact.ImpactPoint;
			EnemyHit.Weapon = this;
			EnemyHit.ShootDir = ShootDir;
			EnemyHit.ShootStart = ShootStart;
			
			PS->NotifyEnemyHit(EnemyHit);
		}
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

	if (IsValid(EquippingPawn) && IsValid(EquippingPawn->GetController()))
	{
		Impact.GetActor()->TakeDamage(PointDmg.Damage, PointDmg, EquippingPawn->GetController(), EquippingPawn);
		
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

	// add small gap to the muzzle location for trail effect
	const FVector Origin = GetMuzzleLocation() + GetMuzzleDirection() * 30;

	UParticleSystemComponent* TrailPSC = UGameplayStatics::SpawnEmitterAtLocation(this, TrailFX, Origin, FRotator::ZeroRotator, FVector(1.0f));
	if (TrailPSC)
	{
		TrailPSC->SetVectorParameter(TrailTargetParam, EndPoint);
	}
}

FHitResult APistolWeapon_Instant::AimTrace(const FVector& TraceFrom, const FVector& TraceTo) const
{
	FHitResult Hit(ForceInit);
	const EDrawDebugTrace::Type DrawDebugType = bDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(EquippingPawn);
	
	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), TraceFrom, TraceTo, AimRadius,
		UEngineTypes::ConvertToTraceType(COLLISION_WEAPON_TRACE), false, ActorsToIgnore, DrawDebugType, Hit, true);

	return Hit;	
}