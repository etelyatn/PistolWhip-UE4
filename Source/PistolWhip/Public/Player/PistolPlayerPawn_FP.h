// 2021 github.com/EugeneTel/PistolWhip-UE4

#pragma once

#include "CoreMinimal.h"
#include "Player/PistolPlayerPawn.h"
#include "PistolPlayerPawn_FP.generated.h"

class APistolWeapon;

/**
 * 
 */
UCLASS(Abstract)
class PISTOLWHIP_API APistolPlayerPawn_FP : public APistolPlayerPawn
{
	GENERATED_BODY()

public:
	APistolPlayerPawn_FP();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	APistolWeapon* GetWeapon() const { return Weapon; };

protected:

	UFUNCTION()
	void EquipWeapon();

private:

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	APistolWeapon* Weapon;

};
