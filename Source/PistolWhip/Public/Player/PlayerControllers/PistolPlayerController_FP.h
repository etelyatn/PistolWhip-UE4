// 2021 github.com/EugeneTel/PistolWhip-UE4

#pragma once

#include "CoreMinimal.h"
#include "Player/PlayerControllers/PistolPlayerController.h"
#include "Player/PistolPlayerPawn_FP.h"
#include "PistolPlayerController_FP.generated.h"

class APistolPlayerPawn_FP;

/**
 * 
 */
UCLASS()
class PISTOLWHIP_API APistolPlayerController_FP : public APistolPlayerController
{
	GENERATED_BODY()
	
public:
	virtual void SetPawn(APawn* InPawn) override;

protected:

	TWeakObjectPtr<APistolPlayerPawn_FP> CachedPawnFP;

	/** Movement speed for body control */
	UPROPERTY(Category=PistolPlayerFP, EditDefaultsOnly)
	float ControlMovementSpeed = 700.0f;

	/** Max Control Deviation for player Right movement */
	UPROPERTY(Category=PistolPlayerFP, EditDefaultsOnly)
	float MaxRightDeviation = 100.f;

	/** Min Control Deviation for player Up movement */
	UPROPERTY(Category=PistolPlayerFP, EditDefaultsOnly)
	float MaxUpDeviation = 90.f;

	/** Min Control Deviation for player Up movement */
	UPROPERTY(Category=PistolPlayerFP, EditDefaultsOnly)
	float MinUpDeviation = -10.f;

	virtual void SetupInputComponent() override;

private:

	/** On Fire press action */
	void OnFire();

	/** On Reload pressed action */
	void OnReload();

	/** Body movement actions */
	void OnMoveUp(const float Val);
	void OnMoveRight(float Val);	
	
};
