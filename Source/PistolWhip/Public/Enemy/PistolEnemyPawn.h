// 2021 github.com/EugeneTel/PistolWhip-UE4

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Weapon/PistolWeapon.h"

#include "PistolEnemyPawn.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEnemyPawnHitDelegate, class APistolEnemyPawn*);

/** enemy configs */
USTRUCT(BlueprintType)
struct FEnemyData
{
	GENERATED_BODY()

	/** is firing enabled for the enemy */
	UPROPERTY(Category=EnemyData, EditAnywhere)
	bool bFiringEnabled;

	/** Delay between firing */
	UPROPERTY(Category=EnemyData, EditAnywhere)
	float FiringDelay;

	/** the weapon will be added to the enemy */
	UPROPERTY(Category=EnemyData, EditAnywhere)
	TSubclassOf<APistolWeapon> WeaponClass;

	FEnemyData()
		: bFiringEnabled(true)
		, FiringDelay(2.0f)
	{}
};

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
	FORCEINLINE void SetEnemyData(FEnemyData& InEnemyData) { EnemyData = InEnemyData; }
	FORCEINLINE FEnemyData& GetEnemyData() { return EnemyData; }

	/** Global notification when an enemy was hit */
	static FOnEnemyPawnHitDelegate OnHit;

	/** check if pawn is still alive */
	bool IsAlive() const;

protected:

	/** enemy settings */
	UPROPERTY(Category=EnemyPawn, EditAnywhere)
	FEnemyData EnemyData;

	/** Pawn living state */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	bool bAlive;

	/** Delay between pawn destroy */
	UPROPERTY(Category=EnemyPawn, EditDefaultsOnly)
	float DestroyDelay;

	UPROPERTY(Category="EnemyPawn|Weapon", EditDefaultsOnly)
	FName WeaponSocketName;
	
	/** Handle for enemy destroy */
	FTimerHandle TimerHandle_Destroy;

	/** Enemy Death Implementation */
	UFUNCTION(BlueprintNativeEvent)
	void Death();

	void DestroyEnemy();

	/** equip the weapon specified in enemy data */
	UFUNCTION(BlueprintCallable)
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
