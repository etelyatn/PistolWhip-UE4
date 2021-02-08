// 2021 github.com/EugeneTel/PistolWhip-UE4

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "PistolTrackPointComponent.generated.h"

/**
 * 
 */
UCLASS(meta=(DisplayName="Track Point", BlueprintSpawnableComponent))
class PISTOLWHIP_API UPistolTrackPointComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	
	UPistolTrackPointComponent();
	virtual void BeginPlay() override;

	/** List of Items for activation */
	UPROPERTY(Category=TrackPoint, EditInstanceOnly)
	TArray<TScriptInterface<class IPistolActivatableInterface>> Items;

	/** Activate Items by interface */
	void ActivateItems();

#if WITH_EDITOR
	/** Draw debug lines to Items */
	void DrawLinesToItems();
#endif


protected:
	UFUNCTION()
	void OnTrackPointBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
