// 2021 github.com/EugeneTel/PistolWhip-UE4

#pragma once

#include "CoreMinimal.h"

#include "MotionControllerComponent.h"
#include "GameFramework/Actor.h"
#include "PistolHandController.generated.h"

class APistolWeapon;

UCLASS()
class PISTOLWHIP_API APistolHandController : public AActor
{
	GENERATED_BODY()
	
public:	
	APistolHandController();

	virtual void Tick(float DeltaTime) override;

	// Set Tracking Source hand
	void SetHand(const EControllerHand InHand) const { MotionController->SetTrackingSource(InHand); }

	/** Get weapon instance */
	APistolWeapon* GetWeapon() const { return Weapon; }

	// Equip a new weapon
	void EquipWeapon(TSubclassOf<APistolWeapon> WeaponClass);

	// Start weapon fire
	void Fire();

	UFUNCTION(BlueprintCallable)
	void SetOwningPawn(APawn* NewOwner);

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	APawn* OwnerPawn;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UMotionControllerComponent* MotionController;

	UPROPERTY(Category=Weapon, VisibleInstanceOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	APistolWeapon* Weapon;

};
