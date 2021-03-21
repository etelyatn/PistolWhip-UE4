// 2021 github.com/EugeneTel/PistolWhip-UE4

#include "Framework/PistolWhipGameModeBase.h"
#include "Player/PistolPlayerPawn.h"
#include "Kismet/GameplayStatics.h"
#include "UI/PistolGameMenuBase.h"

void APistolWhipGameModeBase::ShowInGameMenu(const EMenuType MenuType)
{
	APistolPlayerPawn* PlayerPawn = Cast<APistolPlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	if (!InGameMenu && PlayerPawn->GameMenuClass)
	{
		const FVector WidgetLocation = PlayerPawn->GetActorLocation() + FVector(300.0f, 0.0f, 0.0f);
		const FTransform WidgetTransform(FRotator(0.0f, 180.0f, 0.0f),WidgetLocation, FVector::OneVector);
		
		InGameMenu = Cast<APistolGameMenuBase>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), PlayerPawn->GameMenuClass, WidgetTransform));
        if (InGameMenu)
        {
        	InGameMenu->SetMenuType(MenuType);

        	UGameplayStatics::FinishSpawningActor(InGameMenu, WidgetTransform);

        	OnInGameMenuCreated.Broadcast();
        }
	}	
}

void APistolWhipGameModeBase::HideInGameMenu()
{
	if (InGameMenu)
	{
		InGameMenu->Destroy();

		OnInGameMenuDestroyed.Broadcast();
	}
}
