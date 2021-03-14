// 2021 github.com/EugeneTel/PistolWhip-UE4

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "PistolFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PISTOLWHIP_API UPistolFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/** calculate the angle between two vectors */
	UFUNCTION(BlueprintCallable, Category=PistolWhip)
	static float AngleBetweenTwoVectors(const FVector VectorA, const FVector VectorB);
	
	/** calculate the angle between two vectors */
	UFUNCTION(BlueprintCallable, Category=PistolWhip)
	static float AngleBetweenTwoVectorsInDegrees(const FVector VectorA, const FVector VectorB);

	UFUNCTION(BlueprintCallable, Category = PistolWhip, meta = (WorldContext = "WorldContextObject"))
	static void PistolRestartLevel(UObject* WorldContextObject);
	
};
