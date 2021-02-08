// 2021 github.com/EugeneTel/PistolWhip-UE4

#include "Player/PistolPlayerPawn_VR.h"
#include "Player/PistolHandController.h"
#include "Engine/CollisionProfile.h"
#include "HeadMountedDisplayFunctionLibrary.h"

APistolPlayerPawn_VR::APistolPlayerPawn_VR()
{
	PrimaryActorTick.bCanEverTick = true;
	GameModeType = EGameModeType::GMT_VR;

}

void APistolPlayerPawn_VR::BeginPlay()
{
	Super::BeginPlay();

	// Setup HMD
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition(0, EOrientPositionSelector::OrientationAndPosition);

	if (GetWorld())
	{
		// Spawn controllers and weapons
		LeftController = GetWorld()->SpawnActor<APistolHandController>();
		if (LeftController)
		{
			LeftController->AttachToComponent(BodyRoot, FAttachmentTransformRules::SnapToTargetIncludingScale);
			LeftController->SetHand(EControllerHand::Left);
			LeftController->SetOwningPawn(this);
			LeftController->EquipWeapon(WeaponClass);

		}

		RightController = GetWorld()->SpawnActor<APistolHandController>();
		if (RightController)
		{
			RightController->AttachToComponent(BodyRoot, FAttachmentTransformRules::SnapToTargetIncludingScale);
			RightController->SetHand(EControllerHand::Right);
			RightController->SetOwningPawn(this);
			RightController->EquipWeapon(WeaponClass);
		}
	}
}

void APistolPlayerPawn_VR::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

