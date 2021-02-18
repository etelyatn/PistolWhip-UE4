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
	UPROPERTY(Category=Projectile, EditDefaultsOnly)
	TSubclassOf<class APistolProjectile> ProjectileClass;

	/** life time */
	UPROPERTY(Category=Projectile, EditDefaultsOnly)
	float ProjectileLife;

	/** projectile movement speed */
	UPROPERTY(Category=Projectile, VisibleInstanceOnly)
	float ProjectileSpeed;

	/** how much time needed for the projectile to reach the goal */ 
	UPROPERTY(Category=Projectile, EditDefaultsOnly)
	float GoalReachTime;

	FProjectileWeaponData()
		: ProjectileLife(10.0f)
		, ProjectileSpeed(0.0f)
		, GoalReachTime(2.0f)
	{
	}
};

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class PISTOLWHIP_API APistolWeapon_Projectile : public APistolWeapon
{
	GENERATED_BODY()
public:

	APistolWeapon_Projectile();

	/** Get Projectile Movement speed for weapon */
	float GetProjectileSpeed() const;
	void SetProjectileSpeed(float InSpeed);

	/** Get Projectile goal reach time */
	float GetGoalReachTime() const;
	
	/** Predefined shooting Goal location */
	FORCEINLINE void SetGoalLocation(const FVector InLocation) { GoalLocation = InLocation; }
	FORCEINLINE FVector GetGoalLocation() const { return GoalLocation; }	

protected:

	/** weapon config */
	UPROPERTY(Category="Weapon|Projectile", EditDefaultsOnly)
	FProjectileWeaponData ProjectileConfig;

	virtual void FireWeapon() override;

	/** calculate projectile goal */
	virtual FVector CalculateProjectileGoal();

	void FireProjectile(FVector Origin, FRotator ProjectileDir);

private:
	/** Goal location for shooting */
	FVector GoalLocation;
	
};