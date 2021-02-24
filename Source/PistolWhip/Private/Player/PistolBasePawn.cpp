// 2021 github.com/EugeneTel/PistolWhip-UE4


#include "Player/PistolBasePawn.h"


#include "Log.h"
#include "Gameplay/PistolSplineTrack.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APistolBasePawn::APistolBasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// defaults
	SplineMovementSpeed = 200.0f;
	CurrentTrackDistance = 0.0f;
	MovementType = EPawnMovementType::PMT_Idle;
	bAlive = true;
	StartTimeSeconds = 0.0f;
	SplineLength = 0.0f;
}

// Called when the game starts or when spawned
void APistolBasePawn::BeginPlay()
{
	Super::BeginPlay();

	StartTimeSeconds = UGameplayStatics::GetTimeSeconds(GetWorld());
}

// Called every frame
void APistolBasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CanMove())
	{
		MoveBySpline();
	}
}

void APistolBasePawn::SetSplineComponent(USplineComponent* InSplineComponent)
{
	SplineComponent = InSplineComponent;

	if (SplineComponent)
	{
		SplineLength = SplineComponent->GetSplineLength();
	}
}

FRotator APistolBasePawn::GetCurrentSplineRotation() const
{
	return SplineComponent ? SplineComponent->GetWorldRotationAtTime(CurrentTrackDistance) : FRotator::ZeroRotator;
}

bool APistolBasePawn::CanMove() const
{
	return IsAlive() && MovementType != EPawnMovementType::PMT_Idle && SplineComponent;
}

bool APistolBasePawn::WantsToMove() const
{
	return MovementType != EPawnMovementType::PMT_Idle;
}

float APistolBasePawn::GetMovementSpeed() const
{
	return CanMove() ? SplineMovementSpeed : 0.0f;
}

void APistolBasePawn::FinishSplineMovement()
{
	SetMovementType(EPawnMovementType::PMT_Idle);

	// fires the delegate
	if (OnPawnMovementFinished.IsBound())
	{
		OnPawnMovementFinished.Broadcast();
	}
}

void APistolBasePawn::MoveBySpline()
{
	if (SplineComponent)
	{
		// Calculate track distance
		const float SplineTime = UGameplayStatics::GetTimeSeconds(GetWorld()) - StartTimeSeconds;
		CurrentTrackDistance = GetWorld()->GetDeltaSeconds() + SplineTime * SplineMovementSpeed;

		FVector NewActorLocation = SplineComponent->GetLocationAtDistanceAlongSpline(CurrentTrackDistance, ESplineCoordinateSpace::World);
		NewActorLocation.Z = GetActorLocation().Z;
		SetActorLocation(NewActorLocation);

		// finish spline movement
		if (CurrentTrackDistance >= SplineLength)
		{
			FinishSplineMovement();
		}
	}
}
