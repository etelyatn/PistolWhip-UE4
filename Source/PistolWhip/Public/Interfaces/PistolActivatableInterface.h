// 2021 github.com/EugeneTel/PistolWhip-UE4

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PistolActivatableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPistolActivatableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PISTOLWHIP_API IPistolActivatableInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Activate();
};
