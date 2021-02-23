// 2021 github.com/EugeneTel/PistolWhip-UE4

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "PistolEnemyAIController.generated.h"

class APistolEnemyPawn;
class APistolPlayerPawn;

/**
 * 
 */
UCLASS()
class PISTOLWHIP_API APistolEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void SetPawn(APawn* InPawn) override;
	virtual void BeginPlay() override;
	
	/** Update direction AI is looking on Player */
	virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn = true) override;

	/** Init enemy firing timer */
	void InitFiring();

protected:

	/** Handle for enemy firing for the first shot */
	FTimerHandle TimerHandle_FirstShot;
	
	/** Handle for enemy firing for the other shots */
	FTimerHandle TimerHandle_OtherShots;

	/** Enemy firing to player */
	void Fire();

	/** subscribed to OnPawnMovementFinished */
	void OnMovementFinished();

	/** subscribed to OnPawnMovementFinished */
	void OnWeaponEquipped();

private:
	
	TWeakObjectPtr<APistolEnemyPawn> CachedEnemyPawn;

	TWeakObjectPtr<APistolPlayerPawn> CachedPlayerPawn;
	
};
