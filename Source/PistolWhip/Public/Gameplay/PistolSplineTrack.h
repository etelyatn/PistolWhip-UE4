// 2021 github.com/EugeneTel/PistolWhip-UE4

#pragma once

#include "CoreMinimal.h"

#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "PistolSplineTrack.generated.h"

UCLASS()
class PISTOLWHIP_API APistolSplineTrack : public AActor
{
	GENERATED_BODY()
	
public:	
	APistolSplineTrack();

	UFUNCTION(BlueprintCallable)
	USplineComponent* GetSpline() const { return Spline; };

private:

	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere)
	class USplineComponent* Spline; 
};
