// 2021 github.com/EugeneTel/PistolWhip-UE4

#include "Player/PistolPlayerPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "COmponents/CapsuleComponent.h"
#include "Engine/CollisionProfile.h"
#include "Kismet/GameplayStatics.h"
#include "Track/PistolSplineTrack.h"

APistolPlayerPawn::APistolPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(FName("CapsuleComponent"));
	CapsuleComponent->InitCapsuleSize(34.0f, 90.0f);
	CapsuleComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	RootComponent = CapsuleComponent;

	BodyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("BodyRoot"));
	BodyRoot->SetupAttachment(CapsuleComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(BodyRoot);

	HeadCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HeadCapsule"));
	HeadCapsule->InitCapsuleSize(12.0f, 12.0f);
	HeadCapsule->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	HeadCapsule->SetupAttachment(Camera);

#if WITH_EDITORONLY_DATA
	ArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	if (ArrowComponent)
	{
		ArrowComponent->ArrowColor = FColor(150, 200, 255);
		ArrowComponent->bTreatAsASprite = true;
		ArrowComponent->SetupAttachment(CapsuleComponent);
		ArrowComponent->bIsScreenSizeScaled = true;
	}
#endif // WITH_EDITORONLY_DATA
}

void APistolPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

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

void APistolPlayerPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bMoveBySplineTrack && SplineTrack)
	{
		MoveBySplineTrack();
	}
}

void APistolPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APistolPlayerPawn::MoveBySplineTrack()
{
	if (!SplineTrack || !SplineTrack->GetSpline())
	{
		return;
	}

	// Calculate track distance
	TrackDistance = GetWorld()->GetDeltaSeconds() + UGameplayStatics::GetTimeSeconds(GetWorld()) * SplineTrackSpeed;

	FVector NewActorLocation = SplineTrack->GetSpline()->GetWorldLocationAtDistanceAlongSpline(TrackDistance);
	NewActorLocation.Z = GetActorLocation().Z;
	SetActorLocation(NewActorLocation);
}

