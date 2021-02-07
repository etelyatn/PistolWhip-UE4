// 2021 github.com/EugeneTel/PistolWhip-UE4


#include "Player/PlayerControllers/PistolPlayerController_VR.h"
#include "Player/PistolPlayerPawn_VR.h"
#include "Player/PistolHandController.h"

void APistolPlayerController_VR::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	CachedPawnVR = Cast<APistolPlayerPawn_VR>(InPawn);
}

void APistolPlayerController_VR::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("TriggerLeft", IE_Pressed, this, &APistolPlayerController_VR::OnTriggerLeft);
	InputComponent->BindAction("TriggerRight", IE_Pressed, this, &APistolPlayerController_VR::OnTriggerRight);
}

void APistolPlayerController_VR::OnTriggerLeft()
{
	if (CachedPawnVR.IsValid() && IsValid(CachedPawnVR->GetLeftController()))
	{
		CachedPawnVR->GetLeftController()->Fire();
	}
}

void APistolPlayerController_VR::OnTriggerRight()
{
	if (CachedPawnVR.IsValid() && IsValid(CachedPawnVR->GetLeftController()))
	{
		CachedPawnVR->GetRightController()->Fire();
	}
}
