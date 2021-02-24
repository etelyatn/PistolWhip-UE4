// 2021 github.com/EugeneTel/PistolWhip-UE4

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PistolBasePawn.generated.h"

class USplineComponent;

DECLARE_MULTICAST_DELEGATE(FOnPawnMovementFinishedDelegate)

/** pawn movement type */
UENUM(BlueprintType)
enum class EPawnMovementType: uint8
{
	PMT_Idle UMETA(DisplayName = "Idle"),
    PMT_Run  UMETA(DisplayName = "Run")
};

UCLASS(Abstract)
class PISTOLWHIP_API APistolBasePawn : public APawn
{
	GENERATED_BODY()

public:
	APistolBasePawn();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE void SetMovementType(const EPawnMovementType InMovementType) { MovementType = InMovementType; }
    FORCEINLINE EPawnMovementType GetMovementType() const { return MovementType; }
	
	void SetSplineComponent(USplineComponent* InSplineComponent);
	
	FORCEINLINE USplineComponent* GetSplineComponent() const { return SplineComponent; }

	/** Fires when pawn movement by spline is finished */
	FOnPawnMovementFinishedDelegate OnPawnMovementFinished;
	
	FRotator GetCurrentSplineRotation() const;

	/** check if pawn is still alive */
	FORCEINLINE bool IsAlive() const { return bAlive; };

	/** checks is the pawn can move by all conditions (has spline, movement allowed etc.) */
	UFUNCTION(BlueprintCallable)
	bool CanMove() const;

	/** checks if player wants to move. without ability to move */
	UFUNCTION()
	bool WantsToMove() const;

	/** Get current movement speed along the spline track */
	UFUNCTION(BlueprintCallable)
	virtual float GetMovementSpeed() const;

	/** Calls when spline is finished */
	virtual void FinishSplineMovement();

protected:
	
	/** Pawn living state */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	bool bAlive;

	/** current pawn movement type */
	UPROPERTY(Category=PistolPawn, EditAnywhere, BlueprintReadWrite)
	EPawnMovementType MovementType;

	/** Pawn movement speed along a Spline track */
	UPROPERTY(Category=PistolPawn, EditAnywhere, BlueprintReadWrite)
	float SplineMovementSpeed;

	/** Length of the Spline Track */
	float SplineLength;
	
	/** Distance along the spline track */
	float CurrentTrackDistance;

	/** Pawn movement along a spline component */
	void MoveBySpline();

private:

	/** the time when the actor begins play. used for spline movement */
	float StartTimeSeconds;
	
	/** spline component for pawn movement */
	UPROPERTY()
	USplineComponent* SplineComponent;

};