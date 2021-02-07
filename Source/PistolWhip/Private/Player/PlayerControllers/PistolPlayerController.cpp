// 2021 github.com/EugeneTel/PistolWhip-UE4

#include "Player/PlayerControllers/PistolPlayerController.h"
#include "Player/PistolPlayerPawn.h"

void APistolPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	CachedPawn = Cast<APistolPlayerPawn>(InPawn);
}