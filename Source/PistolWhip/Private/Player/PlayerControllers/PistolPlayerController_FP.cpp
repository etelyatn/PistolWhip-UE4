// 2021 github.com/EugeneTel/PistolWhip-UE4


#include "Player/PlayerControllers/PistolPlayerController_FP.h"

void APistolPlayerController_FP::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	CachedPawnFP = Cast<APistolPlayerPawn_FP>(InPawn);
}

void APistolPlayerController_FP::SetupInputComponent()
{
	Super::SetupInputComponent();
}
