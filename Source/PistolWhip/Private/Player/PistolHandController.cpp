// 2021 github.com/EugeneTel/PistolWhip-UE4


#include "Player/PistolHandController.h"
#include "Weapon/PistolWeapon.h"

APistolHandController::APistolHandController()
{
	PrimaryActorTick.bCanEverTick = false;

	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController"));
	SetRootComponent(MotionController);
	MotionController->SetShowDeviceModel(false);
}

void APistolHandController::EquipWeapon(TSubclassOf<APistolWeapon> WeaponClass)
{
	if (WeaponClass)
	{
		Weapon = GetWorld()->SpawnActor<APistolWeapon>(WeaponClass);
		if (Weapon)
		{
			Weapon->AttachToComponent(MotionController, FAttachmentTransformRules::SnapToTargetIncludingScale);
			Weapon->SetOwningPawn(OwnerPawn);
		}
	}
}

void APistolHandController::Fire()
{
	if (IsValid(Weapon))
	{
		Weapon->StartFire();
	}
}

void APistolHandController::SetOwningPawn(APawn* NewOwner)
{
	SetOwner(NewOwner);
	OwnerPawn = NewOwner;
}

void APistolHandController::BeginPlay()
{
	Super::BeginPlay();
}

void APistolHandController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

