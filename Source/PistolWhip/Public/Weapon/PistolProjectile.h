// 2021 github.com/EugeneTel/PistolWhip-UE4

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"


#include "PistolProjectile.generated.h"

UCLASS()
class PISTOLWHIP_API APistolProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APistolProjectile();

	/** initial setup */
	virtual void PostInitializeComponents() override;

	/** setup velocity */
	void InitVelocity(FVector& ShootDirection);

	/** handle hit */
	UFUNCTION()
    void OnImpact(const FHitResult& HitResult);

protected:

	/** controller that fired me (cache for damage calculations) */
	TWeakObjectPtr<AController> CachedController;

	/** shutdown projectile and prepare for destruction */
	void DisableAndDestroy();

private:

	UPROPERTY(Category=Projectile, VisibleDefaultsOnly)
	class UProjectileMovementComponent* MovementComponent;

	UPROPERTY(Category=Projectile, VisibleDefaultsOnly)
	class USphereComponent* CollisionComponent;

	UPROPERTY(Category=Projectile, VisibleDefaultsOnly)
	class UParticleSystemComponent* ParticleComponent;

	UPROPERTY(Category=Projectile, VisibleDefaultsOnly)
	class UStaticMeshComponent* MeshComponent;
};
