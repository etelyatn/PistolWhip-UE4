// 2021 github.com/EugeneTel/PistolWhip-UE4

#include "Player/PistolPlayerPawn.h"

#include "Log.h"
#include "PistolWhip.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PostProcessComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/PistolSplineTrack.h"
#include "Weapon/PistolProjectile.h"
#include "Enemy/PistolEnemyPawn.h"
#include "Player/PlayerControllers/PistolPlayerController.h"
#include "UI/Widgets/PistolPlayerInterfaceWidget.h"
#include "Framework/PistolWhipGameModeBase.h"


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
	
	CameraWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("CameraWidgetComponent"));
	CameraWidgetComponent->SetupAttachment(Camera);
	CameraWidgetComponent->SetDrawAtDesiredSize(true);
	CameraWidgetComponent->SetGenerateOverlapEvents(false);
	CameraWidgetComponent->SetRelativeLocation(FVector(150.0f, 0.0f, 0.0f));
	CameraWidgetComponent->SetRelativeRotation(FRotator(180.0f, 0.0f, 0.0f));
	CameraWidgetComponent->SetRelativeScale3D(FVector(0.15f));
	CameraWidgetComponent->SetBlendMode(EWidgetBlendMode::Transparent);
	CameraWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CameraWidgetComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	
	InterfaceWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InterfaceWidgetComponent"));
	InterfaceWidgetComponent->SetupAttachment(Camera);
	InterfaceWidgetComponent->SetDrawAtDesiredSize(true);
	InterfaceWidgetComponent->SetGenerateOverlapEvents(false);
	InterfaceWidgetComponent->SetRelativeLocation(FVector(150.0f, 10.0f, -50.0f));
	InterfaceWidgetComponent->SetRelativeRotation(FRotator(180.0f, 0.0f, 0.0f));
	InterfaceWidgetComponent->SetRelativeScale3D(FVector(0.15f));
	InterfaceWidgetComponent->SetBlendMode(EWidgetBlendMode::Transparent);
	InterfaceWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InterfaceWidgetComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

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

void APistolPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	// Set Spline Track
	if (WantsToMove())
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APistolSplineTrack::StaticClass(), FoundActors);
		if (FoundActors.Num() > 0)
		{
			SplineTrack = Cast<APistolSplineTrack>(FoundActors[0]);
			if (SplineTrack)
			{
				SetSplineComponent(SplineTrack->GetSpline());
			}
		}
	}

	// create health component
	if (HealthComponentClass)
	{
		HealthComponent = NewObject<UPistolPlayerHealthComponent>(this, HealthComponentClass, FName("HealthComponent"));
		HealthComponent->SetPlayerPawn(this);
		HealthComponent->SetHealthData(HealthData);
		HealthComponent->SetShieldData(ShieldData);
		HealthComponent->RegisterComponent();
	} else
	{
		UE_LOG(LogPistolWhip, Log, TEXT("APistolPlayerPawn::BeginPlay() - HealthComponentClass is not set!"))
	}

	// cache interface widget
	if (InterfaceWidgetComponent->GetWidget())
	{
		InterfaceWidget = Cast<UPistolPlayerInterfaceWidget>(InterfaceWidgetComponent->GetWidget());
	}

	// subscribe on events
	HeadCapsule->OnComponentBeginOverlap.AddDynamic(this, &APistolPlayerPawn::OnHeadOverlap);
	HealthComponent->OnShieldDestroyed.AddUObject(this, &APistolPlayerPawn::OnShieldDestroyed);
	HealthComponent->OnShieldRestoreProgress.AddUObject(this, &APistolPlayerPawn::OnShieldRestoreProgress);
	HealthComponent->OnShieldFullyRestored.AddUObject(this, &APistolPlayerPawn::OnShieldFullyRestored);
	HealthComponent->OnPlayerDeath.AddUObject(this, &APistolPlayerPawn::OnPlayerDeath);
}

void APistolPlayerPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// update spline track progress UI
	if (InterfaceWidget)
	{
		InterfaceWidget->UpdateProgressBar(CurrentTrackDistance, SplineLength);
	}
}

void APistolPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APistolPlayerPawn::FinishSplineMovement()
{
	Super::FinishSplineMovement();

	// show in game menu
	APistolWhipGameModeBase* GM = Cast<APistolWhipGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (GM)
	{
		GM->ShowInGameMenu(EMenuType::EMT_Finish);
	}

	// pause the game
	APistolPlayerController* PC = Cast<APistolPlayerController>(GetController());
	if (PC)
	{
		PC->SetPause(true);
	}
}

void APistolPlayerPawn::OnHeadOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& OverlapInfo)
{	
	// apply damage on hit something
	if (HealthComponent
		&& !OtherActor->IsA(APistolProjectile::StaticClass())
		&& !OtherActor->IsA(APistolWeapon::StaticClass())
		&& PreviousOverlappedActor != OtherActor
		)
	{
		APistolEnemyPawn* EnemyPawn = Cast<APistolEnemyPawn>(OtherActor);
		if (EnemyPawn && !EnemyPawn->IsAlive())
		{
			return;
		}

		PreviousOverlappedActor = OtherActor;
		HealthComponent->TakeDamage(1.0f);
	}
}

void APistolPlayerPawn::OnShieldDestroyed(const int8 RestoreHits) const
{
	if (InterfaceWidget)
	{
		InterfaceWidget->ShieldDestroyed(RestoreHits);
	}
}

void APistolPlayerPawn::OnShieldRestoreProgress(const int8 CurrentHits) const
{
	if (InterfaceWidget)
	{
		InterfaceWidget->ShieldRestore(CurrentHits);
	}
}

void APistolPlayerPawn::OnShieldFullyRestored() const
{
	if (InterfaceWidget)
	{
		InterfaceWidget->ShieldFullyRestored();
	}
}

void APistolPlayerPawn::OnPlayerDeath()
{
	// show in game menu
	APistolWhipGameModeBase* GM = Cast<APistolWhipGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (GM)
	{
		GM->ShowInGameMenu(EMenuType::EMT_Death);
	}

	// pause the game
	APistolPlayerController* PC = Cast<APistolPlayerController>(GetController());
	if (PC)
	{
			PC->SetPause(true);
	}
}
