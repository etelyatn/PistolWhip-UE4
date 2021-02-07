// 2021 github.com/EugeneTel/PistolWhip-UE4

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PistolEnemySpawner.generated.h"

class APistolEnemyPawn;

UCLASS()
class PISTOLWHIP_API APistolEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APistolEnemySpawner();

	/** Spawn a specified Enemy */
	void Spawn();

protected:

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	class UPistolTrackPointComponent* TrackPoint;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APistolEnemyPawn> EnemyPawnClass;

#if WITH_EDITOR
	
	UPROPERTY()
	class UCapsuleComponent* Capsule;
#endif


private:

	/** An Enemy Pawn instance */
	TWeakObjectPtr<APistolEnemyPawn> EnemyPawn;
};
