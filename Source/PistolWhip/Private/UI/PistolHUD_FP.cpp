// 2021 github.com/EugeneTel/PistolWhip-UE4


#include "UI/PistolHUD_FP.h"
#include "Engine/Canvas.h"

APistolHUD_FP::APistolHUD_FP()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/PistolWhip/Weapons/_common/Textures/FirstPersonCrosshair.FirstPersonCrosshair"));
	CrosshairTexture = CrosshairTexObj.Object;
}

void APistolHUD_FP::DrawHUD()
{
	Super::DrawHUD();

	if (Canvas == nullptr)
	{
		return;
	}

	DrawCrosshair();
}

void APistolHUD_FP::DrawCrosshair()
{
	if (!CrosshairTexture)
	{
		return;
	}
	
	// Draw very simple crosshair

	// find center of the Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	const FVector2D CrosshairDrawPosition( (Center.X) - 8.0f, (Center.Y) - 8.0f);

	// draw the crosshair
	FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTexture->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem( TileItem );
}
