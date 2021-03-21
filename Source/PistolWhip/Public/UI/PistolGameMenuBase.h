// 2021 github.com/EugeneTel/PistolWhip-UE4

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PistolWhipTypes.h"
#include "PistolGameMenuBase.generated.h"

UCLASS()
class PISTOLWHIP_API APistolGameMenuBase : public AActor
{
	GENERATED_BODY()
	
public:

	FORCEINLINE void SetMenuType(const EMenuType InMenuType) { MenuType = InMenuType; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE EMenuType GetMenuType() const { return MenuType; }

private:

	/** Menu type affects on widget behavior (text, buttons, etc.) */
	EMenuType MenuType;

};
