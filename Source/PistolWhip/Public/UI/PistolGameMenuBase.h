// 2021 github.com/EugeneTel/PistolWhip-UE4

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PistolGameMenuBase.generated.h"

UENUM(BlueprintType)
enum class EGameMenuType : uint8
{
	Pause,
	Death
};

UCLASS()
class PISTOLWHIP_API APistolGameMenuBase : public AActor
{
	GENERATED_BODY()
	
public:

	FORCEINLINE void SetMenuType(const EGameMenuType InMenuType) { MenuType = InMenuType; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE EGameMenuType GetMenuType() const { return MenuType; }

private:

	/** Menu type affects on widget behavior (text, buttons, etc.) */
	EGameMenuType MenuType;

};
