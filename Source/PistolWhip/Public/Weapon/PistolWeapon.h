// 2021 github.com/EugeneTel/PistolWhip-UE4

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PistolWeapon.generated.h"

class USoundBase;
class APistolHandController;

USTRUCT()
struct FWeaponData
{
	GENERATED_BODY()

	/** inifite ammo for reloads */
	UPROPERTY(EditDefaultsOnly, Category=Ammo)
	bool bInfiniteAmmo;

	/** clip size */
	UPROPERTY(EditDefaultsOnly, Category=Ammo)
	int32 AmmoPerClip;

	FWeaponData()
		: bInfiniteAmmo(false)
		, AmmoPerClip(15)
	{}
};

UCLASS(Abstract, NotBlueprintable)
class PISTOLWHIP_API APistolWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	APistolWeapon();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void StartFire();

	UFUNCTION(BlueprintCallable)
	void SetOwningPawn(APawn* NewOwner);
	
	/** get the muzzle location of the weapon */
	FVector GetMuzzleLocation() const;

	/** get direction of weapon's muzzle */
	FVector GetMuzzleDirection() const;

	/** check if weapon has infinite ammo (include owner's cheats) */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasInfiniteAmmo() const;

	/** get clip size */
	UFUNCTION(BlueprintCallable)
	int32 GetAmmoPerClip() const;

	/** get current ammo amount (clip) */
	UFUNCTION(BlueprintCallable)
	int32 GetCurrentAmmoInClip() const;
	
	/** performs actual reload */
	UFUNCTION(BlueprintCallable)
	virtual void ReloadWeapon();

	/** checks weapon action for reload */
	void CheckAndHandleReload();

	/** Notify blueprints ammo was updated */
	UFUNCTION(BlueprintImplementableEvent, Category=PistolWhip)
	void NotifyAmmoUpdated(int32 NewAmmo);

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	APawn* OwnerPawn;

	/** the hand controller to which the weapon is attached */
	UPROPERTY()
	APistolHandController* OwnerHandController;
		
	/** Enable debug mode */
	UPROPERTY(Category=Weapon, EditAnywhere)
	bool bDebug;

	UPROPERTY(Category=Weapon, VisibleAnywhere)
	class USceneComponent* SceneComponent;

	UPROPERTY(Category=Weapon, VisibleAnywhere)
	class UBoxComponent* BoxComponent;

	/** Weapon mesh  */
	UPROPERTY(Category=Weapon, VisibleAnywhere)
	USkeletalMeshComponent* MeshComponent;

	/** name of bone/socket for muzzle in weapon mesh */
	UPROPERTY(Category=Weapon, EditDefaultsOnly)
	FName MuzzleName;

	/** fire animation */
	UPROPERTY(Category=Weapon, EditDefaultsOnly)
	UAnimationAsset* FireAnim;

	/** weapon data */
	UPROPERTY(Category=Weapon, EditDefaultsOnly)
	FWeaponData WeaponData;

	/** range of instant weapon */
	UPROPERTY(Category=Weapon, EditDefaultsOnly)
	float WeaponRange;
		
	/** fire haptic feedback for motion controller */
	UPROPERTY(Category="Weapon|Feedbacks", EditDefaultsOnly)
	UHapticFeedbackEffect_Base* FireHapticFeedback;
	
	/** reload weapon haptic feedback for motion controller */
	UPROPERTY(Category="Weapon|Feedbacks", EditDefaultsOnly)
	UHapticFeedbackEffect_Base* ReloadHapticFeedback;

	UPROPERTY(Category="Weapon|Sounds", EditDefaultsOnly)
	USoundBase* NoAmmoSound;

	UPROPERTY(Category="Weapon|Sounds", EditDefaultsOnly)
	USoundBase* ReloadSound;

	/** current ammo - inside clip */
	int32 CurrentAmmoInClip;

#if WITH_EDITORONLY_DATA
	/** Component shown in the editor only to indicate weapon facing */
	UPROPERTY()
	class UArrowComponent* ArrowComponent;
#endif

	/** check if weapon can fire */
	virtual bool CanFire() const;
	
	/** play weapon animations */
	void PlayWeaponAnimation(UAnimationAsset* Animation);

	/** play motion controller haptic feedback */
	void PlayHapticFeedback(UHapticFeedbackEffect_Base* HapticFeedback, float Scale = 1);

	/** play weapon sound cue */
	void PlayWeaponSound(USoundBase* SoundToPlay, float Volume = 1) const;

	/** find hit */
	FHitResult WeaponTrace(const FVector& TraceFrom, const FVector& TraceTo) const;

	/** weapon specific fire implementation */
	virtual void FireWeapon() PURE_VIRTUAL(APistolWeapon::FireWeapon,);

	/** get the originating location for damage */
	FVector GetDamageStartLocation() const;

	/** Get the aim of the weapon, allowing for adjustments to be made by the weapon */
	virtual FVector GetAdjustedAim() const;

	/** consume a bullet */
	void UseAmmo();

	/** check if weapon can be reloaded */
	bool CanReload() const;

};
