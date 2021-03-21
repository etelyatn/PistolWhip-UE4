// 2021 github.com/EugeneTel/PistolWhip-UE4

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PistolWhipTypes.h"
#include "PistolWhipGameModeBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInGameMenuCreated)
DECLARE_MULTICAST_DELEGATE(FOnInGameMenuDestroyed)

/**
 * 
 */
UCLASS()
class PISTOLWHIP_API APistolWhipGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	FOnInGameMenuCreated OnInGameMenuCreated;
	FOnInGameMenuDestroyed OnInGameMenuDestroyed;

	void ShowInGameMenu(const EMenuType MenuType);

	void HideInGameMenu();

protected:

	UPROPERTY()
	class APistolGameMenuBase* InGameMenu;
};
