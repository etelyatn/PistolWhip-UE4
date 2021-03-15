// 2021 github.com/EugeneTel/PistolWhip-UE4

#pragma once

#include "CoreMinimal.h"

#include "Components/WidgetInteractionComponent.h"
#include "GameFramework/PlayerController.h"
#include "UI/PistolGameMenuBase.h"
#include "PistolPlayerController.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInGameMenuCreated)
DECLARE_MULTICAST_DELEGATE(FOnInGameMenuDestroyed)

/**
 * 
 */
UCLASS(Abstract)
class PISTOLWHIP_API APistolPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void SetPawn(APawn* InPawn) override;

	virtual void SetupInputComponent() override;

	FOnInGameMenuCreated OnInGameMenuCreated;
	FOnInGameMenuDestroyed OnInGameMenuDestroyed;

	/** do widget interaction action */
	virtual void WidgetInteractionAction(UWidgetInteractionComponent* WidgetInteraction);

protected:

	/** on pause action pressed */
	void OnPausePressed();

	/** show game menu and set game paused */
	void PauseGame();

	/** hide game menu and unpause the game */
	void UnPauseGame();

	/** exactly pauses the game */
	void SetGamePaused();

	/** create InGameMenu actor */
	void CreateInGameMenu(EGameMenuType MenuType);

	/** destroy InGameMenu actor */
	void DestroyInGameMenu();

private:
	TWeakObjectPtr<class APistolPlayerPawn> CachedPawn;	

	UPROPERTY()
	class APistolGameMenuBase* InGameMenu;	
};
