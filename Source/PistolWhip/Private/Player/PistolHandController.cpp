// 2021 github.com/EugeneTel/PistolWhip-UE4


#include "Player/PistolHandController.h"

#include "Components/WidgetInteractionComponent.h"
#include "Weapon/PistolWeapon.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerControllers/PistolPlayerController.h"

class APistolPlayerController;

APistolHandController::APistolHandController()
{
	PrimaryActorTick.bCanEverTick = false;

	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController"));
	SetRootComponent(MotionController);
	MotionController->SetShowDeviceModel(false);

	WidgetInteraction = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("InteractionComponent"));
    WidgetInteraction->SetupAttachment(MotionController);
	WidgetInteraction->SetRelativeRotation(FRotator(-30.0f, 0.0f, 0.0f));
	WidgetInteraction->SetRelativeLocation(FVector(0,0, 4.f));
	WidgetInteraction->bShowDebug = false;
    WidgetInteraction->Deactivate();
}

void APistolHandController::EquipWeapon(TSubclassOf<APistolWeapon> WeaponClass)
{
	if (WeaponClass)
	{
		Weapon = Cast<APistolWeapon>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), WeaponClass, FTransform()));
		if (Weapon)
		{
			Weapon->AttachToComponent(MotionController, FAttachmentTransformRules::SnapToTargetIncludingScale);
			Weapon->EquippedBy(OwnerPawn);

			UGameplayStatics::FinishSpawningActor(Weapon, FTransform());
		}
	}
}

void APistolHandController::Fire()
{
	if (IsValid(Weapon))
	{
		Weapon->StartFire();
	}

	if (OwnerPawn)
	{
		// try to do widget interaction action
		APistolPlayerController* PC = Cast<APistolPlayerController>(OwnerPawn->GetController());
		if (PC)
		{
			PC->WidgetInteractionAction(WidgetInteraction);
		}
	}
}

void APistolHandController::SetOwningPawn(APistolBasePawn* NewOwner)
{
	SetOwner(NewOwner);
	OwnerPawn = NewOwner;

	APistolPlayerController* PC = Cast<APistolPlayerController>(OwnerPawn->GetController());
	if (PC)
	{
		PC->OnInGameMenuCreated.AddUObject(this, &APistolHandController::OnInGameMenuCreated);
		PC->OnInGameMenuDestroyed.AddUObject(this, &APistolHandController::OnInGameMenuDestroyed);
	}
}

void APistolHandController::BeginPlay()
{
	Super::BeginPlay();
}

void APistolHandController::PlayHapticFeedback(UHapticFeedbackEffect_Base* HapticFeedback, const float Scale) const
{
	if (OwnerPawn && MotionController && HapticFeedback)
	{
		APlayerController* PC = Cast<APlayerController>(OwnerPawn->GetController());
		if (PC)
		{
			PC->PlayHapticEffect(HapticFeedback, MotionController->GetTrackingSource(), Scale);
		}
	}
}

void APistolHandController::OnInGameMenuCreated()
{
	if (MotionController && MotionController->GetTrackingSource() == EControllerHand::Right)
	{
		WidgetInteraction->Activate();
		WidgetInteraction->bShowDebug = true;
	}

}

void APistolHandController::OnInGameMenuDestroyed()
{
	if (MotionController && MotionController->GetTrackingSource() == EControllerHand::Right)
	{
		WidgetInteraction->Deactivate();
		WidgetInteraction->bShowDebug = false;
	}
}

void APistolHandController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

