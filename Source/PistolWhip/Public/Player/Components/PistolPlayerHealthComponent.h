// 2021 github.com/EugeneTel/PistolWhip-UE4

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PistolPlayerHealthComponent.generated.h"

class APistolPlayerPawn;

DECLARE_MULTICAST_DELEGATE(FOnPlayerDeathDelegate);

/** Structure for storing health data and health state */
USTRUCT(BlueprintType)
struct FPlayerHealthData
{
	GENERATED_BODY()

	/** current player living status */
	UPROPERTY(Category=PlayerHealth, VisibleInstanceOnly)
	bool bAlive;

	/** Current player's health */
	UPROPERTY(Category=PlayerHealth, VisibleInstanceOnly)
	float CurrentHealth;

	/** Max player's health */
	UPROPERTY(Category=PlayerHealth, EditDefaultsOnly)
	float MaxHealth;
	
	/** Is player immortal or not */
	UPROPERTY(Category=PlayerHealth, EditDefaultsOnly)
	bool bImmortal;

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
	      , CurrentHealth(0)
		  , MaxHealth(2.0f)
		  , bImmortal(false)
		  , DamageOverlayMaterial(nullptr)
		  , DamageOverlayMaterialInstance(nullptr)
		  , DamageOverlayParameter(FName("Visibility"))
	{
	}
};


UCLASS(Abstract, ClassGroup=(Custom), Blueprintable)
class PISTOLWHIP_API UPistolPlayerHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UPistolPlayerHealthComponent();
	virtual void BeginPlay() override;
	
	FORCEINLINE void SetPlayerPawn(APistolPlayerPawn* InPawn) { PlayerPawn = InPawn; }
	
	/** Fired when the player is dead */
	FOnPlayerDeathDelegate OnPlayerDeath;

	/** Health component register a damage */
	UFUNCTION(BlueprintCallable)
	void TakeDamage(float Amount);

	/** Is current player state alive */
	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;

	/** Setup config data for health component */
	void SetHealthData(FPlayerHealthData& InHealthData);

protected:

	/** current health configs and state */
	UPROPERTY(VisibleDefaultsOnly)
	FPlayerHealthData HealthData;

	UPROPERTY(BlueprintReadOnly)
	APistolPlayerPawn* PlayerPawn;

	/** player death */
	void Death();

	/** blueprint damage notification */
	UFUNCTION(BlueprintImplementableEvent, Category=PlayerHealth)
	void NotifyTakeDamage();

	/** blueprint death notification */
	UFUNCTION(BlueprintImplementableEvent, Category=PlayerHealth)
	void NotifyDeath();

	UFUNCTION(BlueprintCallable, Category=PlayerHealth)
	void CreateDamageOverlay();

	UFUNCTION(BlueprintCallable, Category=PlayerHealth)
	void SetDamageOverlayVisibility(float Value);

};