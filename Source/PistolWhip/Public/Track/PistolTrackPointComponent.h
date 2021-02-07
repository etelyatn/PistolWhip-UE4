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

	/** List of Enemy Spawners for activation */
	UPROPERTY(Category=TrackPoint, EditInstanceOnly)
	TArray<class APistolEnemySpawner*> Spawners;

#if WITH_EDITOR
	/** Draw debug lines to Spawners */
	void DrawSpawnerLines();
#endif

	
};
