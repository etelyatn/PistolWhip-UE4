// 2021 github.com/EugeneTel/PistolWhip-UE4

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Player/PistolPlayerPawn.h"

#include "PistolPlayerInterfaceWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class PISTOLWHIP_API UPistolPlayerInterfaceWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	
	/** Update Player Score */
	void UpdateScore(int32 Score) const;

	/** shield destroyed visualization */
	void ShieldDestroyed(int8 RestoreHits);

	/** shield restore progress visualization */
	void ShieldRestore(int8 CurrentHits) const;

	/** shield fully restored */
	void ShieldFullyRestored() const;

	/** player spline track progress */
	void UpdateProgressBar(float CurrentPosition, float SplineLength) const;

protected:

	/** shield text for showing the number of hits to recharge the shield */ 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ShieldText;

	/** shield image for visual representation the state of the shield */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* ShieldImage;

	/** player progress in the track spline */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* ProgressBar;

	/** current player score visualization */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ScoreText;

	/** Widget Constructor */
	virtual void NativeConstruct() override;

private:

	UPROPERTY()
	APistolPlayerPawn* PlayerPawn;
	
	/** shield restore hits visual representation */
	int8 ShieldRestoreHits;
};
