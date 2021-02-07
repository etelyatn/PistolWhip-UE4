// 2021 github.com/EugeneTel/PistolWhip-UE4

#include "Player/PistolVRPawn.h"

#include "Player/PistolHandController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/CollisionProfile.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "MotionControllerComponent.h"
#include "Track/PistolSplineTrack.h"
#include "Kismet/GameplayStatics.h"

APistolVRPawn::APistolVRPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRRoot"));
	VRRoot->SetupAttachment(RootComponent);

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->InitCapsuleSize(12.0f, 12.0f);
	CapsuleComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	CapsuleComponent->SetupAttachment(VRRoot);

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

		// Set Spline Track
		if (bMoveBySplineTrack)
		{
			TArray<AActor*> FoundActors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), APistolSplineTrack::StaticClass(), FoundActors);
			if (FoundActors.Num() > 0)
			{
				SplineTrack = Cast<APistolSplineTrack>(FoundActors[0]);
			}
		}
	}
}

void APistolVRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bMoveBySplineTrack && SplineTrack)
	{
		MoveBySplineTrack();
	}

	CapsuleComponent->SetRelativeLocation(Camera->GetRelativeLocation());
}

void APistolVRPawn::MoveBySplineTrack()
{
	if (!SplineTrack || !SplineTrack->GetSpline())
	{
		return;
	}

	TrackDistance = GetWorld()->GetDeltaSeconds() + UGameplayStatics::GetTimeSeconds(GetWorld()) * SplineTrackSpeed;
	FVector NewActorLocation = SplineTrack->GetSpline()->GetWorldLocationAtDistanceAlongSpline(TrackDistance);
	NewActorLocation.Z = GetActorLocation().Z;
	SetActorLocation(NewActorLocation);
}

