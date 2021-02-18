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

	/** Handle for enemy firing */
	FTimerHandle TimerHandle_Firing;

	/** Enemy firing to player */
	void Fire();

private:
	
	TWeakObjectPtr<APistolEnemyPawn> CachedEnemyPawn;

	TWeakObjectPtr<APistolPlayerPawn> CachedPlayerPawn;
	
};
