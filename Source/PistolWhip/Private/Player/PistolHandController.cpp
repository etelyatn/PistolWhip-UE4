// 2021 github.com/EugeneTel/PistolWhip-UE4


#include "Player/PistolHandController.h"

APistolHandController::APistolHandController()
{
	PrimaryActorTick.bCanEverTick = false;

	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController"));
	SetRootComponent(MotionController);
	MotionController->SetShowDeviceModel(true);
}

void APistolHandController::BeginPlay()
{
	Super::BeginPlay();
}

void APistolHandController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

