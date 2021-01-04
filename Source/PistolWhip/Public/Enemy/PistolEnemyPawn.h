// 2021 github.com/EugeneTel/PistolWhip-UE4

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PistolEnemyPawn.generated.h"

UCLASS()
class PISTOLWHIP_API APistolEnemyPawn : public APawn
{
	GENERATED_BODY()

public:
	APistolEnemyPawn();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	USkeletalMeshComponent* GetMesh() const { return Mesh; }

protected:
	virtual void BeginPlay() override;

	/** Enemy Death Implementation */
	void Death();

	void DestroyEnemy();

	UPROPERTY(Category=Config, EditDefaultsOnly)
	float DestroyDelay = 2.0f;
	
	/** Handle for enemy destroy */
	FTimerHandle TimerHandle_Destroy;

private:

	/** The CapsuleComponent being used for movement collision */
	UPROPERTY(Category=Pawn, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComponent;

	/** The main skeletal mesh associated with this Enemy (optional sub-object). */
	UPROPERTY(Category=Pawn, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh;

#if WITH_EDITORONLY_DATA
	/** Component shown in the editor only to indicate character facing */
	UPROPERTY()
	class UArrowComponent* ArrowComponent;
#endif
};
