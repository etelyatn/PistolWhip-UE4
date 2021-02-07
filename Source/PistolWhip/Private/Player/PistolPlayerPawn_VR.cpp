// 2021 github.com/EugeneTel/PistolWhip-UE4

#include "Player/PistolPlayerPawn_VR.h"

#include "Player/PistolHandController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/CollisionProfile.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "MotionControllerComponent.h"
#include "Kismet/GameplayStatics.h"

APistolPlayerPawn_VR::APistolPlayerPawn_VR()
{
	PrimaryActorTick.bCanEverTick = true;

	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRRoot"));
	VRRoot->SetupAttachment(RootComponent);

	HeadCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HeadCapsule"));
	HeadCapsule->InitCapsuleSize(12.0f, 12.0f);
	HeadCapsule->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	HeadCapsule->SetupAttachment(VRRoot);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(VRRoot);
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
			LeftController->AttachToComponent(VRRoot, FAttachmentTransformRules::SnapToTargetIncludingScale);
			LeftController->SetHand(EControllerHand::Left);
			LeftController->SetOwningPawn(this);
			LeftController->EquipWeapon(WeaponClass);

		}

		RightController = GetWorld()->SpawnActor<APistolHandController>();
		if (RightController)
		{
			RightController->AttachToComponent(VRRoot, FAttachmentTransformRules::SnapToTargetIncludingScale);
			RightController->SetHand(EControllerHand::Right);
			RightController->SetOwningPawn(this);
			RightController->EquipWeapon(WeaponClass);
		}
	}
}

void APistolPlayerPawn_VR::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// adjust head capsule location to the camera
	HeadCapsule->SetRelativeLocation(Camera->GetRelativeLocation());
}

