// 2021 github.com/EugeneTel/PistolWhip-UE4

#include "Enemy//PistolEnemySpawner.h"

#include "Log.h"
#include "Components/BillboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/PistolEnemyPawn.h"

// Sets default values
APistolEnemySpawner::APistolEnemySpawner()
{

	RootComponent = CreateDefaultSubobject<USceneComponent>(FName("RootComponent"));

#if WITH_EDITOR
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(FName("Capsule"));
	Capsule->SetupAttachment(GetRootComponent());
	Capsule->SetCapsuleHalfHeight(92.0);
	Capsule->SetCapsuleRadius(30.f);
	Capsule->ShapeColor = FColor::Red;
	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	static FName CollisionProfileName(TEXT("NoCollision"));
	Capsule->SetCollisionProfileName(CollisionProfileName);
	Capsule->SetGenerateOverlapEvents(false);
	
#endif

	// defaults
	bActiveOnBegin = false;

}

void APistolEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	// auto activation
	if (bActiveOnBegin)
	{
		Activate_Implementation();
	}
}

void APistolEnemySpawner::Spawn()
{
	if (GetWorld() && IsValid(EnemyPawnClass))
	{
		// const FRotator EnemyRotation = FRotator::ZeroRotator;
		// APistolEnemyPawn* NewEnemy = GetWorld()->SpawnActor<APistolEnemyPawn>(EnemyPawnClass, GetActorLocation(), EnemyRotation);

		APistolEnemyPawn* NewEnemy = Cast<APistolEnemyPawn>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), EnemyPawnClass, GetActorTransform()));
		if (NewEnemy)
		{
			NewEnemy->InitEnemyConfig(EnemyConfig);

			UGameplayStatics::FinishSpawningActor(NewEnemy, GetActorTransform());
		}

	}
}

void APistolEnemySpawner::Activate_Implementation()
{
	Spawn();
}
