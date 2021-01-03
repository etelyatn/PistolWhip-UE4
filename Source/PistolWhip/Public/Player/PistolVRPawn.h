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

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	APistolHandController* GetLeftController() const { return LeftController; };
	
	APistolHandController* GetRightController() const { return RightController; };
	
protected:
	virtual void BeginPlay() override;
	
private:

	UPROPERTY(Category=Weapon, EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TSubclassOf<APistolWeapon> WeaponClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	class USceneComponent* VRRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	APistolHandController* LeftController;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	APistolHandController* RightController;
};
