// 2021 github.com/EugeneTel/PistolWhip-UE4

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerController.h"
#include "PistolPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PISTOLWHIP_API APistolPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void SetPawn(APawn* InPawn) override;

protected:
	virtual void SetupInputComponent() override;

private:
	TSoftObjectPtr<class APistolVRPawn> CachedPawn;

	void OnTriggerLeft();
	void OnTriggerRight();
	
};
