// 2021 github.com/EugeneTel/PistolWhip-UE4

#pragma once

#include "CoreMinimal.h"
#include "Weapon/PistolWeapon.h"
#include "PistolWeapon_Instant.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PISTOLWHIP_API APistolWeapon_Instant : public APistolWeapon
{
	GENERATED_BODY()

public:

	virtual void StartFire() override;
};
