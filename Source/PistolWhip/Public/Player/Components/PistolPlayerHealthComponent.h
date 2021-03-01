// 2021 github.com/EugeneTel/PistolWhip-UE4

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "PistolPlayerHealthComponent.generated.h"

class APistolPlayerPawn;
class APistolEnemyPawn;

DECLARE_MULTICAST_DELEGATE(FOnPlayerDeathDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnShieldDestroyedDelegate, int8);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnShieldRestoreProgressDelegate, int8);
DECLARE_MULTICAST_DELEGATE(FOnShieldFullyRestoredDelegate);

/** storing shield information */
USTRUCT(BlueprintType)
struct FPlayerShieldData
{
	GENERATED_BODY()

	/** is the shield active */
	UPROPERTY(Category=PlayerHealth, EditDefaultsOnly)
	bool bActive;

	/** is the shield fully charged */
	UPROPERTY(Category=PlayerHealth, VisibleInstanceOnly)
	bool bDestroyed;
	
	/** Current player's shield */
	UPROPERTY(Category=PlayerHealth, VisibleInstanceOnly)
	float CurrentShield;

	/** Max player's shield */
	UPROPERTY(Category=PlayerHealth, EditDefaultsOnly)
	float MaxShield;

	/** How many hits before the shield is restored  */
	UPROPERTY(Category=PlayerHealth, EditDefaultsOnly)
	int8 RestoreHits;

	/** How many hits for the shield restoring */
	UPROPERTY(Category=PlayerHealth, VisibleInstanceOnly)
	int8 CurrentHits;

	/** Reset current shield state to max shield state */
	void ResetShield()
	{
		CurrentShield = MaxShield;
		CurrentHits = 0;
		bDestroyed = false;
	}

	FPlayerShieldData()
		: bActive(true)
		  , bDestroyed(false)
		  , CurrentShield(0.0f)
		  , MaxShield(1.0f)
		  , RestoreHits(5)
		  , CurrentHits(0)
	{
	}
};

/** storing health data and health state */
USTRUCT(BlueprintType)
struct FPlayerHealthData
{
	GENERATED_BODY()

	/** current player living status */
	UPROPERTY(Category=PlayerHealth, VisibleInstanceOnly)
	bool bAlive;

	/** Is player immortal or not */
	UPROPERTY(Category=PlayerHealth, EditDefaultsOnly)
	bool bImmortal;

	/** Current player's health */
	UPROPERTY(Category=PlayerHealth, VisibleInstanceOnly)
	float CurrentHealth;

	/** Max player's health */
	UPROPERTY(Category=PlayerHealth, EditDefaultsOnly)
	float MaxHealth;

	/** the overlay will be set to the player's postprocess */
	UPROPERTY(Category=PlayerHealth, EditDefaultsOnly)
	class UMaterialInterface* DamageOverlayMaterial;
	
	/** instance of the Damage Overlay Material */
	UPROPERTY(Category=PlayerHealth, BlueprintReadOnly)
	class UMaterialInstanceDynamic* DamageOverlayMaterialInstance;

	/** the material instance parameter name for damage overlay visibility */
	UPROPERTY(Category=PlayerHealth, EditDefaultsOnly)
	FName DamageOverlayParameter;

	/** Reset current health to max health */
	void ResetHealth()
	{
		CurrentHealth = MaxHealth;
	}

	FPlayerHealthData()
		: bAlive(true)
		  , bImmortal(false)
		  , CurrentHealth(0)
		  , MaxHealth(1.0f)
		  , DamageOverlayMaterial(nullptr)
		  , DamageOverlayMaterialInstance(nullptr)
		  , DamageOverlayParameter(FName("Visibility"))
	{}
};


UCLASS(Abstract, ClassGroup=(Custom), Blueprintable)
class PISTOLWHIP_API UPistolPlayerHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UPistolPlayerHealthComponent();
	virtual void BeginPlay() override;
	
	void SetPlayerPawn(APistolPlayerPawn* InPawn);
	
	/** Fired when the player is dead */
	FOnPlayerDeathDelegate OnPlayerDeath;

	/** fired when the shield was destroyed */
	FOnShieldDestroyedDelegate OnShieldDestroyed;

	/** fired on the shield restore progress */
	FOnShieldRestoreProgressDelegate OnShieldRestoreProgress;

	/** fired when the shield is fully restored */
	FOnShieldFullyRestoredDelegate OnShieldFullyRestored;

	/** Health component register a damage */
	UFUNCTION(BlueprintCallable)
	void TakeDamage(float Amount);

	/** Is current player state alive */
	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;

	/** Setup config data for health component */
	void SetHealthData(FPlayerHealthData& InHealthData);

	/** Setup config data for shield */
	void SetShieldData(FPlayerShieldData& InShieldData);

protected:

	/** current health configs and state */
	UPROPERTY(VisibleDefaultsOnly)
	FPlayerHealthData HealthData;

	/** current shield configs and state */
	UPROPERTY(VisibleDefaultsOnly)
	FPlayerShieldData ShieldData;

	UPROPERTY(BlueprintReadOnly)
	APistolPlayerPawn* PlayerPawn;

	/** player death */
	void Death();

	/** destroy the shield */
	void DestroyShield();

	/** the shield is fully restored */
	void ShieldFullyRestored();

	/** restore the shield for amount */ 
	void RestoreShield(int Hits);

	/** blueprint damage notification */
	UFUNCTION(BlueprintImplementableEvent, Category=PlayerHealth)
	void NotifyTakeDamage();

	/** blueprint death notification */
	UFUNCTION(BlueprintImplementableEvent, Category=PlayerHealth)
	void NotifyDeath();

	/** blueprint shield destroyed notification */
	UFUNCTION(BlueprintImplementableEvent, Category=PlayerHealth)
	void NotifyShieldDestroyed();

	/** blueprint shield restoring notification */
	UFUNCTION(BlueprintImplementableEvent, Category=PlayerHealth)
	void NotifyShieldFullyRestored();

	/** creates damage overlay material instance and attach to post process */
	UFUNCTION(BlueprintCallable, Category=PlayerHealth)
	void CreateDamageOverlay();

	/** Update damage overlay material instance visibility property */
	UFUNCTION(BlueprintCallable, Category=PlayerHealth)
	void SetDamageOverlayVisibility(float Value);

private:

	/** On enemy hit handler */
	void OnEnemyHit(APistolEnemyPawn* EnemyPawn);

};