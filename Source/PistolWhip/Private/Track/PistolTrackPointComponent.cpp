// 2021 github.com/EugeneTel/PistolWhip-UE4

#include "Track/PistolTrackPointComponent.h"
#include "DrawDebugHelpers.h"
#include "Log.h"
#include "Gameplay/PistolEnemySpawner.h"

UPistolTrackPointComponent::UPistolTrackPointComponent()
{
	LineThickness = 1.0;
	BoxExtent = FVector(10.0f);
	ShapeColor = FColor::Blue;
}

#if WITH_EDITOR
void UPistolTrackPointComponent::DrawSpawnerLines()
{
	const UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	for (APistolEnemySpawner* Spawner : Spawners)
	{
		if (IsValid(Spawner))
		{
			DrawDebugLine(World, GetComponentLocation(), Spawner->GetActorLocation() + FVector(0.0f, 0.0f, 92.0f), FColor::Green, false, -1, 0, 1.0f);
		}

	}
}
#endif
