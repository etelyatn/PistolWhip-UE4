// 2021 github.com/EugeneTel/PistolWhip-UE4

#include "Gameplay/PistolEnemySpawner.h"

#include "Log.h"
#include "Components/BillboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/PistolPlayerPawn.h"
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

}

void APistolEnemySpawner::Spawn()
{
	if (GetWorld() && IsValid(EnemyPawnClass))
	{
		APistolPlayerPawn* PlayerPawn = Cast<APistolPlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

		if (PlayerPawn)
		{
			const FRotator EnemyRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation());

			APistolEnemyPawn* NewEnemy = GetWorld()->SpawnActor<APistolEnemyPawn>(EnemyPawnClass, GetActorLocation(), EnemyRotation);
		}
	}

}

void APistolEnemySpawner::Activate_Implementation()
{
	Spawn();
}
