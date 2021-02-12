// 2021 github.com/EugeneTel/PistolWhip-UE4


#include "Weapon/PistolProjectile.h"

#include "Log.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PistolPlayerPawn.h"

const FName APistolProjectile::ProjectileCollisionProfileName(TEXT("Projectile"));

APistolProjectile::APistolProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(5.0f);
	CollisionComponent->bTraceComplexOnMove = true;
	CollisionComponent->SetCollisionProfileName(ProjectileCollisionProfileName);
	SetRootComponent(CollisionComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = CollisionComponent;
	MovementComponent->InitialSpeed = 400.0f;
	MovementComponent->MaxSpeed = 400.0f;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->ProjectileGravityScale = 0.f;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComponent);
}

void APistolProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MovementComponent->OnProjectileStop.AddDynamic(this, &APistolProjectile::OnImpact);
	CollisionComponent->MoveIgnoreActors.Add(GetInstigator());

	SetLifeSpan(10.f);
	CachedController = GetInstigatorController();
}

void APistolProjectile::InitProjectileSpeed(const float InSpeed) const
{
	MovementComponent->MaxSpeed = InSpeed;
	MovementComponent->InitialSpeed = InSpeed;
}

void APistolProjectile::InitVelocity(FVector& ShootDirection)
{
	if (IsValid(MovementComponent))
	{
		MovementComponent->Velocity = ShootDirection * MovementComponent->InitialSpeed;
	}
}

void APistolProjectile::OnImpact(const FHitResult& HitResult)
{
	APistolPlayerPawn* PlayerPawn = Cast<APistolPlayerPawn>(HitResult.GetActor());
	if (PlayerPawn)
	{
		const FDamageEvent DamageEvent;
		PlayerPawn->TakeDamage(1.0, DamageEvent, CachedController.Get(), CachedController->GetPawn());
	}

	DisableAndDestroy();
}

void APistolProjectile::DisableAndDestroy()
{
	Destroy();
}
