// 2021 github.com/EugeneTel/PistolWhip-UE4


#include "UI/Widgets/PistolPlayerInterfaceWidget.h"

#include "Log.h"
#include "Components/TextBlock.h"

void UPistolPlayerInterfaceWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ShieldText->SetVisibility(ESlateVisibility::Hidden);
	ScoreText->SetText(FText::FromString("x0"));
	ProgressBar->SetPercent(0.0f);
}

void UPistolPlayerInterfaceWidget::UpdateScore(const int32 Score) const
{
	if (ScoreText)
	{
		ScoreText->SetText(FText::FromString(FString::Printf(TEXT("x%d"), Score)));
	}
}

void UPistolPlayerInterfaceWidget::ShieldDestroyed(const int8 RestoreHits)
{
	ShieldRestoreHits = RestoreHits;
	if (ShieldImage)
	{
		ShieldImage->SetColorAndOpacity(FLinearColor::Red);
	}

	if (ShieldText)
	{
		ShieldText->SetVisibility(ESlateVisibility::Visible);
		ShieldText->SetText(FText::FromString(FString::FromInt(RestoreHits)));
	}
}

void UPistolPlayerInterfaceWidget::ShieldRestore(const int8 CurrentHits) const
{
	if (ShieldText)
	{
		const int8 RemainHits = ShieldRestoreHits - CurrentHits;
		ShieldText->SetText(FText::FromString(FString::FromInt(RemainHits)));
	}
}

void UPistolPlayerInterfaceWidget::ShieldFullyRestored() const
{
	if (ShieldImage)
	{
		ShieldImage->SetColorAndOpacity(FLinearColor::White);
	}

	if (ShieldText)
	{
		ShieldText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UPistolPlayerInterfaceWidget::UpdateProgressBar(const float CurrentPosition, const float SplineLength) const
{
	if (ProgressBar)
	{
		const float Percent = CurrentPosition / SplineLength;
		ProgressBar->SetPercent(Percent);
	}
}
