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

	/** for DrawLines() only */
	virtual bool ShouldTickIfViewportsOnly() const override;

	virtual void BeginPlay() override;

#if WITH_EDITOR
	/** Draw debug lines between Track Points and Enemy Spawners */
	void DrawLines() const;
#endif

	virtual void Tick(float DeltaSeconds) override;
	
private:

	/** Should draw debug lines */
	UPROPERTY(EditAnywhere)
	bool bDrawLines = true; 

	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere)
	class USplineComponent* Spline; 
};
