// 2021 github.com/EugeneTel/PistolWhip-UE4

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PistolPlayerPawn.generated.h"

class UCapsuleComponent;
class UArrowComponent;

UCLASS(Abstract)
class PISTOLWHIP_API APistolPlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	/** Sets default values for this pawn's properties */
	APistolPlayerPawn();

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** spline track for pawn movement */
	UPROPERTY(BlueprintReadWrite)
	class APistolSplineTrack* SplineTrack;

	/** should a pawn move along a spline track */
	UPROPERTY(Category=PistolPlayer, EditDefaultsOnly)
	bool bMoveBySplineTrack = true;

	/** Pawn movement speed along a Spline track */
	UPROPERTY(Category=PistolPlayer, EditDefaultsOnly, BlueprintReadWrite)
	float SplineTrackSpeed = 200.0f;
	
	/** Distance along the spline track */
	float TrackDistance = 0.0f;

	/** Pawn movement along a spline track */
	void MoveBySplineTrack();

private:

	/** The CapsuleComponent being used for movement collision. */
	UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleComponent;

#if WITH_EDITORONLY_DATA
	/** Component shown in the editor only to indicate character facing */
	UPROPERTY()
	UArrowComponent* ArrowComponent;
#endif

};
