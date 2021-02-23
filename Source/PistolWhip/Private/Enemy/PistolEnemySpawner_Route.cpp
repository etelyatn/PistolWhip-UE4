// 2021 github.com/EugeneTel/PistolWhip-UE4


#include "Enemy/PistolEnemySpawner_Route.h"

APistolEnemySpawner_Route::APistolEnemySpawner_Route()
{
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SplineComponent->SetupAttachment(RootComponent);
	EnemyConfig.SplineComponent = SplineComponent;
}

void APistolEnemySpawner_Route::BeginPlay()
{
	Super::BeginPlay();
}
