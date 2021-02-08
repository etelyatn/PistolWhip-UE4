// 2021 github.com/EugeneTel/PistolWhip-UE4

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/PistolActivatableInterface.h"

#include "PistolEnemySpawner.generated.h"

class APistolEnemyPawn;

UCLASS()
class PISTOLWHIP_API APistolEnemySpawner : public AActor, public IPistolActivatableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APistolEnemySpawner();

	/** Spawn a specified Enemy */
	void Spawn();

	/** PistolActivatableInterface */
	virtual void Activate_Implementation() override;

protected:

	UPROPERTY(EditAnywhere)
	TSubclassOf<APistolEnemyPawn> EnemyPawnClass;

#if WITH_EDITOR
	/** Capsule for spawner visualization */
	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* Capsule;
#endif


private:

	/** An Enemy Pawn instance */
	TWeakObjectPtr<APistolEnemyPawn> EnemyPawn;
};
