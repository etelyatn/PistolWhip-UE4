// 2021 github.com/EugeneTel/PistolWhip-UE4

#pragma once

#include "CoreMinimal.h"

#include "PistolPlayerPawn.h"
#include "PistolPlayerPawn_VR.generated.h"

class APistolHandController;

UCLASS(Abstract)
class PISTOLWHIP_API APistolPlayerPawn_VR : public APistolPlayerPawn
{
	GENERATED_BODY()

public:
	APistolPlayerPawn_VR();

	virtual void Tick(float DeltaTime) override;

	APistolHandController* GetLeftController() const { return LeftController; };
	
	APistolHandController* GetRightController() const { return RightController; };
	
protected:

	virtual void BeginPlay() override;
	
private:

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	APistolHandController* LeftController;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	APistolHandController* RightController;

};
