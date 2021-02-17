// 2021 github.com/EugeneTel/PistolWhip-UE4

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Weapon/PistolWeapon.h"

#include "PistolEnemyPawn.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEnemyPawnHitDelegate, class APistolEnemyPawn*);

UCLASS()
class PISTOLWHIP_API APistolEnemyPawn : public APawn
{
	GENERATED_BODY()

public:
	APistolEnemyPawn();
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	FORCEINLINE USkeletalMeshComponent* GetMesh() const { return Mesh; }
	FORCEINLINE APistolWeapon* GetWeapon() const { return Weapon; }

	/** Global notification when an enemy was hit */
	static FOnEnemyPawnHitDelegate OnHit;

	/** check if pawn is still alive */
	bool IsAlive() const;

protected:

	/** Pawn living state */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	bool bAlive = true;

	/** Delay between pawn destroy */
	UPROPERTY(Category=EnemyPawn, EditDefaultsOnly)
	float DestroyDelay = 2.0f;

	UPROPERTY(Category="EnemyPawn|Weapon", EditDefaultsOnly)
	FName WeaponSocketName;

	UPROPERTY(Category="EnemyPawn|Weapon", EditDefaultsOnly)
	TSubclassOf<APistolWeapon> WeaponClass;
	
	/** Handle for enemy destroy */
	FTimerHandle TimerHandle_Destroy;

	/** Enemy Death Implementation */
	UFUNCTION(BlueprintNativeEvent)
	void Death();

	void DestroyEnemy();

	void EquipWeapon();

private:

	/** The CapsuleComponent being used for movement collision */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComponent;

	/** The main skeletal mesh associated with this Enemy (optional sub-object). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleInstanceOnly)
	APistolWeapon* Weapon;

#if WITH_EDITORONLY_DATA
	/** Component shown in the editor only to indicate character facing */
	UPROPERTY()
	class UArrowComponent* ArrowComponent;
#endif
};
