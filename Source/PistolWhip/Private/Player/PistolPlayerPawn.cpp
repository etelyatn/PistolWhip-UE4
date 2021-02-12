// 2021 github.com/EugeneTel/PistolWhip-UE4

#include "Player/PistolPlayerPawn.h"

#include "Log.h"
#include "PistolWhip.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PostProcessComponent.h"
#include "Engine/CollisionProfile.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/PistolSplineTrack.h"
#include "Weapon/PistolProjectile.h"
#include "Weapon/PistolWeapon_Instant.h"


const FName APistolPlayerPawn::HeadCollisionProfileName(TEXT("PlayerHead"));
const FName APistolPlayerPawn::BodyCollisionProfileName(TEXT("PlayerBody"));

APistolPlayerPawn::APistolPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(FName("CapsuleComponent"));
	CapsuleComponent->InitCapsuleSize(34.0f, 90.0f);
	CapsuleComponent->SetCollisionProfileName(BodyCollisionProfileName);
	RootComponent = CapsuleComponent;

	BodyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("BodyRoot"));
	BodyRoot->SetupAttachment(CapsuleComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(BodyRoot);

	HeadCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HeadCapsule"));
	HeadCapsule->InitCapsuleSize(12.0f, 12.0f);
	HeadCapsule->SetCollisionProfileName(HeadCollisionProfileName);
	HeadCapsule->SetupAttachment(Camera);

	PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));
	PostProcessComponent->SetupAttachment(GetRootComponent());

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

float APistolPlayerPawn::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (HealthComponent)
	{
		HealthComponent->TakeDamage(DamageAmount);
	}
	
	return DamageAmount;
}

FVector APistolPlayerPawn::GetHeadLocation() const
{
	return HeadCapsule->GetComponentLocation();
}

float APistolPlayerPawn::GetMovementSpeed() const
{
	return bMoveBySplineTrack ? SplineTrackSpeed : 0.0f;
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

	// create health component
	if (HealthComponentClass)
	{
		HealthComponent = NewObject<UPistolPlayerHealthComponent>(this, HealthComponentClass, FName("HealthComponent"));
		HealthComponent->RegisterComponent();
		HealthComponent->SetPlayerPawn(this);
		HealthComponent->SetHealthData(HealthData);		
	} else
	{
		UE_LOG(LogPistolWhip, Log, TEXT("APistolPlayerPawn::BeginPlay() - HealthComponentClass is not set!"))
	}

	// subscribe on events
	HeadCapsule->OnComponentBeginOverlap.AddDynamic(this, &APistolPlayerPawn::OnHeadOverlap);
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

void APistolPlayerPawn::OnHeadOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& OverlapInfo)
{
	// apply damage on hit something
	if (!OtherActor->IsA(APistolProjectile::StaticClass()) && !OtherActor->IsA(APistolWeapon_Instant::StaticClass()))
	{
		if (HealthComponent)
		{
			HealthComponent->TakeDamage(1.0f);
		}
	}
}
