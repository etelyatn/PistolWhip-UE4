// 2021 github.com/EugeneTel/PistolWhip-UE4

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PistolEnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PISTOLWHIP_API UPistolEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	UPistolEnemyAnimInstance();

	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:

	/** Enemy movement speed */
	UPROPERTY(BlueprintReadOnly)
	float Speed;

private:

	/** cached enemy pawn instance */
	TWeakObjectPtr<class APistolEnemyPawn> CachedEnemyPawn;
	
};
