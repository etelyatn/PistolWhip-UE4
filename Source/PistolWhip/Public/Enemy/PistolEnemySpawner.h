// 2021 github.com/EugeneTel/PistolWhip-UE4

#pragma once

#include "CoreMinimal.h"

#include "PistolEnemyPawn.h"
#include "GameFramework/Actor.h"
#include "Interfaces/PistolActivatableInterface.h"

#include "PistolEnemySpawner.generated.h"

class APistolEnemyPawn;
class USplineComponent;

UCLASS()
class PISTOLWHIP_API APistolEnemySpawner : public AActor, public IPistolActivatableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APistolEnemySpawner();
	virtual void BeginPlay() override;

	/** Spawn a specified Enemy */
	virtual void Spawn();

	/** PistolActivatableInterface */
	virtual void Activate_Implementation() override;

protected:

	/** is the spawner active on begin play */
	UPROPERTY(Category=EnemySpawner, EditAnywhere)
	bool bActiveOnBegin;
	
	/** enemy class for spawning */
	UPROPERTY(Category=EnemySpawner, EditAnywhere)
	TSubclassOf<APistolEnemyPawn> EnemyPawnClass;

	/** spawned enemy configs */
	UPROPERTY(Category=EnemySpawner, EditAnywhere)
	FEnemyData EnemyConfig;

#if WITH_EDITOR
	/** Capsule for spawner visualization */
	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* Capsule;
#endif


private:

	/** An Enemy Pawn instance */
	TWeakObjectPtr<APistolEnemyPawn> EnemyPawn;
};
