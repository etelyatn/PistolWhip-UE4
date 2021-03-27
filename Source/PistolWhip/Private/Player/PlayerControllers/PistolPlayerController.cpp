// 2021 github.com/EugeneTel/PistolWhip-UE4

#include "Player/PlayerControllers/PistolPlayerController.h"
#include "Components/WidgetInteractionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PistolPlayerPawn.h"
#include "PistolWhipTypes.h"
#include "Framework/PistolWhipGameModeBase.h"

void APistolPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	CachedPawn = Cast<APistolPlayerPawn>(InPawn);
}

void APistolPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Pause", IE_Pressed, this, &APistolPlayerController::OnPausePressed);
}

void APistolPlayerController::WidgetInteractionAction(UWidgetInteractionComponent* WidgetInteraction)
{
	if (WidgetInteraction && WidgetInteraction->IsActive())
	{
		WidgetInteraction->PressPointerKey(EKeys::LeftMouseButton);
	}
}

void APistolPlayerController::OnPausePressed()
{
	if (!CachedPawn->IsAlive())
	{
		return;
	}
	
	if (IsPaused())
	{
		UnPauseGame();
	}
	else
	{
		PauseGame();
	}
}

void APistolPlayerController::PauseGame()
{
	// show the pause menu widget
	if (CachedPawn.IsValid())
	{
		APistolWhipGameModeBase* GM = Cast<APistolWhipGameModeBase>(UGameplayStatics::GetGameMode(this));
		if (GM)
		{
			GM->ShowInGameMenu(EMenuType::EMT_Pause);
		}
	}

	// give some time for displaying the widget and pause the game
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &APistolPlayerController::SetGamePaused, 0.1f, false);
}

void APistolPlayerController::UnPauseGame()
{
	APistolWhipGameModeBase* GM = Cast<APistolWhipGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (GM)
	{
		GM->HideInGameMenu();
	}

	SetPause(false);
}

void APistolPlayerController::SetGamePaused()
{
	SetPause(true);
}