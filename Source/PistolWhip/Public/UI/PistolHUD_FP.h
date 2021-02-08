// 2021 github.com/EugeneTel/PistolWhip-UE4

#pragma once

#include "CoreMinimal.h"
#include "UI/PistolHUD.h"
#include "PistolHUD_FP.generated.h"

/**
 * 
 */
UCLASS()
class PISTOLWHIP_API APistolHUD_FP : public APistolHUD
{
	GENERATED_BODY()

public:
	APistolHUD_FP();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

protected:
	/** Draw Crosshair on HUD */
	void DrawCrosshair();

private:
	/** Crosshair asset pointer */
	UTexture2D* CrosshairTexture;
};
