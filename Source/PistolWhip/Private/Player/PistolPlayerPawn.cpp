// 2021 github.com/EugeneTel/PistolWhip-UE4

#include "Player/PistolPlayerPawn.h"

#include "Components/ArrowComponent.h"
#include "COmponents/CapsuleComponent.h"
#include "Engine/CollisionProfile.h"
#include "Kismet/GameplayStatics.h"
#include "Track/PistolSplineTrack.h"

APistolPlayerPawn::APistolPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(FName("CapsuleComponent"));
	CapsuleComponent->InitCapsuleSize(34.0f, 88.0f);
	CapsuleComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	RootComponent = CapsuleComponent;

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

	// Calculate track distance
	TrackDistance = GetWorld()->GetDeltaSeconds() + UGameplayStatics::GetTimeSeconds(GetWorld()) * SplineTrackSpeed;

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

	FVector NewActorLocation = SplineTrack->GetSpline()->GetWorldLocationAtDistanceAlongSpline(TrackDistance);
	NewActorLocation.Z = GetActorLocation().Z;
	SetActorLocation(NewActorLocation);
}

