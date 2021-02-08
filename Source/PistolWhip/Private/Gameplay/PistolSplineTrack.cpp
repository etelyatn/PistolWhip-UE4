// 2021 github.com/EugeneTel/PistolWhip-UE4


#include "Gameplay/PistolSplineTrack.h"
#include "Components/SplineComponent.h"
#include "Gameplay/PistolTrackPointComponent.h"
#include "Log.h"

APistolSplineTrack::APistolSplineTrack()
{
#if WITH_EDITOR
	PrimaryActorTick.bCanEverTick = true;
#endif

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetupAttachment(RootComponent);
}

bool APistolSplineTrack::ShouldTickIfViewportsOnly() const
{
	return true;
}

void APistolSplineTrack::BeginPlay()
{
	Super::BeginPlay();

	bDrawLines = false;
}

#if WITH_EDITOR
void APistolSplineTrack::DrawLines() const
{
	// Draw Spawner lines for each Track Point
	TArray<UPistolTrackPointComponent*> TrackPoints;
	GetComponents<UPistolTrackPointComponent>(TrackPoints);

	for (UPistolTrackPointComponent* TrackPoint : TrackPoints)
	{
		if (IsValid(TrackPoint))
		{
			TrackPoint->DrawLinesToItems();
		}
	}
}
#endif

void APistolSplineTrack::Tick(float DeltaSeconds)
{
#if WITH_EDITORONLY_DATA
	if (bDrawLines)
	{
		DrawLines();
	}
#endif
}
