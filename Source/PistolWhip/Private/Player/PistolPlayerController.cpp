// 2021 github.com/EugeneTel/PistolWhip-UE4


#include "Player/PistolPlayerController.h"
#include "Player/PistolVRPawn.h"
#include "Player/PistolHandController.h"

void APistolPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	CachedPawn = Cast<APistolVRPawn>(InPawn);
}

void APistolPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("TriggerLeft", IE_Pressed, this, &APistolPlayerController::OnTriggerLeft);
	InputComponent->BindAction("TriggerRight", IE_Pressed, this, &APistolPlayerController::OnTriggerRight);
}

void APistolPlayerController::OnTriggerLeft()
{
	if (CachedPawn.IsValid() && IsValid(CachedPawn->GetLeftController()))
	{
		CachedPawn->GetLeftController()->Fire();
	}
}

void APistolPlayerController::OnTriggerRight()
{
	if (CachedPawn.IsValid() && IsValid(CachedPawn->GetLeftController()))
	{
		CachedPawn->GetRightController()->Fire();
	}
}
