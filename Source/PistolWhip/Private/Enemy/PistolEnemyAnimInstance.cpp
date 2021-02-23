// 2021 github.com/EugeneTel/PistolWhip-UE4


#include "Enemy/PistolEnemyAnimInstance.h"
#include "Enemy/PistolEnemyPawn.h"

UPistolEnemyAnimInstance::UPistolEnemyAnimInstance()
{
	// defaults
	Speed = 0.0f;
}

void UPistolEnemyAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	checkf(TryGetPawnOwner()->IsA<APistolEnemyPawn>(), TEXT("UPistolEnemyAnimInstance::NativeBeginPlay() PistolEnemyAnimInstance can be used with Enemy Pawn only!"));
	CachedEnemyPawn = StaticCast<APistolEnemyPawn*>(TryGetPawnOwner());
}

void UPistolEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (CachedEnemyPawn.IsValid())
	{
		Speed = CachedEnemyPawn->GetMovementSpeed();
	}
}
