// 2021 github.com/EugeneTel/PistolWhip-UE4

#pragma once

#include "CoreMinimal.h"

#include "Components/SplineComponent.h"
#include "Player/PistolBasePawn.h"
#include "Weapon/PistolWeapon.h"

#include "PistolEnemyPawn.generated.h"

class APistolEnemyPawn;

DECLARE_MULTICAST_DELEGATE(FOnEnemyPawnWeaponEquippedDelegate)

/** enemy configs */
USTRUCT(BlueprintType)
struct FEnemyData
{
	GENERATED_BODY()

	/** is firing enabled for the enemy */
	UPROPERTY(Category=EnemyConfig, EditAnywhere)
	bool bFiringEnabled;

	/** Delay range for the first shot */
	UPROPERTY(Category=EnemyConfig, EditAnywhere)
	FVector2D FirstShotDelayRange;

	/** Delay between other shots */
	UPROPERTY(Category=EnemyConfig, EditAnywhere)
	FVector2D OtherShotsDelayRange;

	/** Minimum and maximum score player get */
	UPROPERTY(Category=EnemyConfig, EditAnywhere)
	FIntPoint ScoreRange;
	
	UPROPERTY(Category=EnemyConfig, EditAnywhere)
	EPawnMovementType MovementType;

	UPROPERTY()
	USplineComponent* SplineComponent;

	/** the weapon will be added to the enemy */
	UPROPERTY(Category=EnemyData, EditAnywhere)
	TSubclassOf<APistolWeapon> WeaponClass;

	FEnemyData()
		: bFiringEnabled(true)
		  , FirstShotDelayRange(FVector2D(0.5f, 1.0f))
		  , OtherShotsDelayRange(FVector2D(2.0f, 4.0f))
		  , ScoreRange(FIntPoint(10, 100))
		  , MovementType(EPawnMovementType::PMT_Idle)
	      , SplineComponent(nullptr)
	{
	}
};


/** On Enemy Hit uses for calculations (ex. score) */
USTRUCT()
struct FEnemyHit
{
	GENERATED_BODY()

	UPROPERTY()
	bool bBlockingHit;

	UPROPERTY()
	FVector ShootStart;

	UPROPERTY()
	FVector ShootDir;

	UPROPERTY()
	FVector ImpactPoint;

	UPROPERTY()
	APistolEnemyPawn* EnemyPawn;

	UPROPERTY()
	APistolWeapon* Weapon;

	FEnemyHit()
        : bBlockingHit(false)
          , EnemyPawn(nullptr)
          , Weapon(nullptr)
	{
	}
};

UCLASS()
class PISTOLWHIP_API APistolEnemyPawn : public APistolBasePawn
{
	GENERATED_BODY()

public:
	APistolEnemyPawn();
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	FORCEINLINE USkeletalMeshComponent* GetMesh() const { return Mesh; }
	FORCEINLINE APistolWeapon* GetWeapon() const { return Weapon; }

	FORCEINLINE FEnemyData& GetEnemyConfig() { return EnemyConfig; }

	/** Set all enemy configs */
	void InitEnemyConfig(FEnemyData& InEnemyConfig);

	/** checks is firing enabled for the enemy */
	UFUNCTION(BlueprintCallable)
	bool IsFiringEnabled() const;

	/** notify BP */
	UFUNCTION(BlueprintImplementableEvent)
	void NotifyEnemyHit(const int32 HitScore);

	/** notification when an enemy equips a weapon */
	FOnEnemyPawnWeaponEquippedDelegate OnWeaponEquipped;

protected:

	/** enemy settings */
	UPROPERTY(Category=EnemyPawn, EditAnywhere)
	FEnemyData EnemyConfig;

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

	/** Destroy enemy instance */
	void DestroyEnemy();

	/** equip the weapon specified in enemy data. called from blueprint */
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
