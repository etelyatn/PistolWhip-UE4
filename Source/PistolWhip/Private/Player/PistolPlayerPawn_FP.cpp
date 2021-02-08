// 2021 github.com/EugeneTel/PistolWhip-UE4


#include "Player/PistolPlayerPawn_FP.h"

#include "Camera/CameraComponent.h"
#include "Player/PistolHandController.h"
#include "Weapon/PistolWeapon.h"

APistolPlayerPawn_FP::APistolPlayerPawn_FP()
{
	GetCamera()->bUsePawnControlRotation = true;
	GameModeType = EGameModeType::GMT_FP;
}

void APistolPlayerPawn_FP::BeginPlay()
{
	Super::BeginPlay();

	EquipWeapon();

	GetCamera()->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));
}

void APistolPlayerPawn_FP::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void APistolPlayerPawn_FP::EquipWeapon()
{
	if (GetWorld() && WeaponClass)
	{
		Weapon = GetWorld()->SpawnActor<APistolWeapon>(WeaponClass);
		if (Weapon)
		{
			Weapon->AttachToComponent(Camera, FAttachmentTransformRules::SnapToTargetIncludingScale);
			Weapon->SetOwningPawn(this);
			Weapon->SetActorRelativeLocation(FVector(55.0f, 27.0f, -17.0f));
			Weapon->SetActorRelativeRotation(FRotator(35.0f, -10.0f, -1.0f));
		}
	}
}
