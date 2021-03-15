// 2021 github.com/EugeneTel/PistolWhip-UE4

#include "Player/PlayerControllers/PistolPlayerController.h"

#include "Log.h"
#include "Components/WidgetInteractionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PistolPlayerPawn.h"
#include "UI/PistolGameMenuBase.h"

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
		CreateInGameMenu(EGameMenuType::Pause);
	}

	// give some time for displaying the widget and pause the game
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &APistolPlayerController::SetGamePaused, 0.1f, false);
}

void APistolPlayerController::UnPauseGame()
{
	DestroyInGameMenu();

	SetPause(false);
}

void APistolPlayerController::SetGamePaused()
{
	SetPause(true);
}

void APistolPlayerController::CreateInGameMenu(const EGameMenuType MenuType)
{
	if (CachedPawn->GameMenuClass)
	{
		const FVector WidgetLocation = CachedPawn->GetActorLocation() + FVector(300.0f, 0.0f, 0.0f);
		const FTransform WidgetTransform(FRotator(0.0f, 180.0f, 0.0f),WidgetLocation, FVector::OneVector);
		
		InGameMenu = Cast<APistolGameMenuBase>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), CachedPawn->GameMenuClass, WidgetTransform));
        if (InGameMenu)
        {
        	InGameMenu->SetMenuType(MenuType);

        	UGameplayStatics::FinishSpawningActor(InGameMenu, WidgetTransform);

        	OnInGameMenuCreated.Broadcast();
        }
	}
}

void APistolPlayerController::DestroyInGameMenu()
{
	if (InGameMenu)
	{
		InGameMenu->Destroy();

		OnInGameMenuDestroyed.Broadcast();
	}
}
