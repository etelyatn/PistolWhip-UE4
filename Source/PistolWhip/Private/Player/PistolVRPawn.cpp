// 2021 github.com/EugeneTel/PistolWhip-UE4

#include "Player/PistolVRPawn.h"

#include "Player/PistolHandController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/CollisionProfile.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "MotionControllerComponent.h"

APistolVRPawn::APistolVRPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->InitCapsuleSize(12.0f, 12.0f);
	CapsuleComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	RootComponent = CapsuleComponent;

	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRRoot"));
	VRRoot->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(VRRoot);
}

void APistolVRPawn::BeginPlay()
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
			LeftController->SetOwner(this);
			LeftController->SetHand(EControllerHand::Left);
			LeftController->EquipWeapon(WeaponClass);
		}

		RightController = GetWorld()->SpawnActor<APistolHandController>();
		if (RightController)
		{
			RightController->AttachToComponent(VRRoot, FAttachmentTransformRules::SnapToTargetIncludingScale);
			RightController->SetOwner(this);
			RightController->SetHand(EControllerHand::Right);
			RightController->EquipWeapon(WeaponClass);
		}
	}
}

void APistolVRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Fix root position to the camera
	const FVector NewCameraOffset = Camera->GetComponentLocation() - GetActorLocation();
	AddActorWorldOffset(NewCameraOffset);
	VRRoot->AddWorldOffset(-NewCameraOffset);
}

void APistolVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

