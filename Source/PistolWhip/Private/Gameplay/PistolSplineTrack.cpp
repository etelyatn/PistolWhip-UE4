// 2021 github.com/EugeneTel/PistolWhip-UE4


#include "Gameplay/PistolSplineTrack.h"

#include "Components/SplineComponent.h"

APistolSplineTrack::APistolSplineTrack()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetupAttachment(RootComponent);
}
