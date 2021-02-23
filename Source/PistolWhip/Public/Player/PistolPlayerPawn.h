// 2021 github.com/EugeneTel/PistolWhip-UE4

#pragma once

#include "CoreMinimal.h"

#include "PistolBasePawn.h"
#include "PistolWhipTypes.h"
#include "Components/PistolPlayerHealthComponent.h"
#include "PistolPlayerPawn.generated.h"

class UCapsuleComponent;
class UArrowComponent;
class UCameraComponent;
class UPostProcessComponent;

DECLARE_MULTICAST_DELEGATE(FOnEnemyHitDelegate)

UCLASS(Abstract)
class PISTOLWHIP_API APistolPlayerPawn : public APistolBasePawn
{
	GENERATED_BODY()

public:
	/** Sets default values for this pawn's properties */
	APistolPlayerPawn();
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	/** Returns Camera subobject */
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }
	FORCEINLINE EGameModeType GetGameModeType() const { return GameModeType; }
	FORCEINLINE UPostProcessComponent* GetPostProcessComponent() const { return PostProcessComponent; }

	/** Get current head location */
	FVector GetHeadLocation() const;

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** The CapsuleComponent being used for movement collision. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCapsuleComponent* HeadCapsule;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USceneComponent* BodyRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPostProcessComponent* PostProcessComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UWidgetComponent* CameraWidget;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	class UPistolPlayerHealthComponent* HealthComponent;

	/** spline track for pawn movement */
	UPROPERTY(BlueprintReadWrite)
	class APistolSplineTrack* SplineTrack;

	UPROPERTY(Category=PistolPlayer, EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class APistolWeapon> WeaponClass;

	UPROPERTY(Category="PistolPlayer|Health", EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UPistolPlayerHealthComponent> HealthComponentClass;

	UPROPERTY(Category="PistolPlayer|Health", EditDefaultsOnly, BlueprintReadOnly)
	FPlayerHealthData HealthData;
	
	UPROPERTY(Category="PistolPlayer|Health", EditDefaultsOnly, BlueprintReadOnly)
	FPlayerShieldData ShieldData;

	/** collision names */
	static const FName HeadCollisionProfileName;
	static const FName BodyCollisionProfileName;

	/** Game Mode Type of current player */
	EGameModeType GameModeType = EGameModeType::GMT_Invalid;

#if WITH_EDITORONLY_DATA
	/** Component shown in the editor only to indicate character facing */
	UPROPERTY()
	UArrowComponent* ArrowComponent;
#endif
	

	UFUNCTION()
	void OnHeadOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& OverlapInfo);

private:

	
	TWeakObjectPtr<AActor> PreviousOverlappedActor;
};
