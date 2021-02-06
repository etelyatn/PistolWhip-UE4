// 2021 github.com/EugeneTel/PistolWhip-UE4

#pragma once

#include "CoreMinimal.h"
#include "Weapon/PistolWeapon.h"
#include "PistolWeapon_Projectile.generated.h"

USTRUCT()
struct FProjectileWeaponData
{
	GENERATED_BODY()

	/** projectile class */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class APistolProjectile> ProjectileClass;

	/** life time */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	float ProjectileLife = 10.0f;
};

/**
 * 
 */
UCLASS(Blueprintable)
class PISTOLWHIP_API APistolWeapon_Projectile : public APistolWeapon
{
	GENERATED_BODY()

protected:

	/** weapon config */
	UPROPERTY(EditDefaultsOnly, Category=Config)
	FProjectileWeaponData ProjectileConfig;

	virtual void FireWeapon() override;

	/** calculate projectile goal */
	virtual FVector GetProjectileGoal();

	void FireProjectile(FVector Origin, FRotator ProjectileDir);
	
};