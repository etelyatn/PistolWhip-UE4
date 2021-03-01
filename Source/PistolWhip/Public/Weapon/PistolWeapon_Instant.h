// 2021 github.com/EugeneTel/PistolWhip-UE4

#pragma once

#include "CoreMinimal.h"

#include "Player/PistolPlayerState.h"
#include "Weapon/PistolWeapon.h"
#include "PistolWeapon_Instant.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class PISTOLWHIP_API APistolWeapon_Instant : public APistolWeapon
{
	GENERATED_BODY()

public:
	APistolWeapon_Instant();

	FORCEINLINE float GetAimRadius() const { return AimRadius; }

protected:

	/** damage amount */
	UPROPERTY(Category="Weapon|Instant", EditDefaultsOnly)
	int32 HitDamage;

	/** should weapon use the aiming */
	UPROPERTY(Category="Weapon|Instant", EditDefaultsOnly)
	bool bUseAim;

	/** Radius of a collision trace sphere for aiming */
	UPROPERTY(Category="Weapon|Instant", EditDefaultsOnly)
	float AimRadius;

	/** Hit Magnitude for impulse calculation */
	UPROPERTY(Category="Weapon|Instant", EditDefaultsOnly)
	float HitMagnitude;

	/** type of damage */
	UPROPERTY(Category="Weapon|Instant", EditDefaultsOnly)
	TSubclassOf<UDamageType> DamageType;

	/** smoke trail */
	UPROPERTY(Category="Weapon|Instant", EditDefaultsOnly)
	UParticleSystem* TrailFX;

	/** param name for beam target in smoke trail */
	UPROPERTY(Category="Weapon|Instant", EditDefaultsOnly)
	FName TrailTargetParam;

	/** weapon specific fire implementation */
	virtual void FireWeapon() override;

	/** process the instant hit */
	void ProcessInstantHit(const FHitResult& Impact, const FVector& ShootStart,  const FVector& ShootEnd, const FVector& ShootDir);

	/** check if weapon should deal damage to actor */
	bool ShouldDealDamage(AActor* TestActor) const;

	/** handle damage */
	void DealDamage(const FHitResult& Impact, const FVector& ShootDir);

	/** spawn trail effect */
	void SpawnTrailEffect(const FVector& EndPoint) const;

	/** aim helper trace */
	FHitResult AimTrace(const FVector& TraceFrom, const FVector& TraceTo) const;

private:

	// cached player state
	TWeakObjectPtr<APistolPlayerState> PlayerState;
};
