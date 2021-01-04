// 2021 github.com/EugeneTel/PistolWhip-UE4

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Weapon/PistolWeapon.h"
#include "PistolVRPawn.generated.h"

class APistolHandController;

UCLASS()
class PISTOLWHIP_API APistolVRPawn : public APawn
{
	GENERATED_BODY()

public:
	APistolVRPawn();

	virtual void Tick(float DeltaTime) override;

	APistolHandController* GetLeftController() const { return LeftController; };
	
	APistolHandController* GetRightController() const { return RightController; };
	
protected:
	/** spline track for pawn movement */
	UPROPERTY(BlueprintReadWrite)
	class APistolSplineTrack* SplineTrack;

	UPROPERTY(Category=Config, EditDefaultsOnly)
	bool bMoveBySplineTrack = true;

	/** Pawn movement speed along Spline track */
	UPROPERTY(Category=Config, EditDefaultsOnly, BlueprintReadWrite)
	float SplineTrackSpeed = 200.0f;

	virtual void BeginPlay() override;

	void MoveBySplineTrack();
	
private:

	UPROPERTY(Category=Weapon, EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TSubclassOf<APistolWeapon> WeaponClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	class USceneComponent* VRRoot;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	APistolHandController* LeftController;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	APistolHandController* RightController;

	/** Distance along the spline track */
	float TrackDistance = 0.0f;
};
