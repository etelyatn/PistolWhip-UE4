// 2021 github.com/EugeneTel/PistolWhip-UE4

#pragma once

#include "CoreMinimal.h"
#include "Player/PlayerControllers/PistolPlayerController.h"
#include "PistolPlayerController_VR.generated.h"

/**
 * 
 */
UCLASS()
class PISTOLWHIP_API APistolPlayerController_VR : public APistolPlayerController
{
	GENERATED_BODY()
public:
	virtual void SetPawn(APawn* InPawn) override;

protected:

	virtual void SetupInputComponent() override;

	TWeakObjectPtr<class APistolPlayerPawn_VR> CachedPawnVR;
	
	void OnTriggerLeft();
	void OnTriggerRight();
	
};
