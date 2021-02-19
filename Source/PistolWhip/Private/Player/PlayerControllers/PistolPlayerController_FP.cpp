// 2021 github.com/EugeneTel/PistolWhip-UE4


#include "Player/PlayerControllers/PistolPlayerController_FP.h"
#include "Weapon/PistolWeapon.h"
#include "Camera/CameraComponent.h"

void APistolPlayerController_FP::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	CachedPawnFP = Cast<APistolPlayerPawn_FP>(InPawn);
}

void APistolPlayerController_FP::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Fire", IE_Pressed, this, &APistolPlayerController_FP::OnFire);
	InputComponent->BindAction("Reload", IE_Pressed, this, &APistolPlayerController_FP::OnReload);

	InputComponent->BindAxis("Turn", this, &APlayerController::AddYawInput);
	InputComponent->BindAxis("LookUp", this, &APlayerController::AddPitchInput);
	InputComponent->BindAxis("MoveUp", this, &APistolPlayerController_FP::OnMoveUp);
	InputComponent->BindAxis("MoveRight", this, &APistolPlayerController_FP::OnMoveRight);
}

void APistolPlayerController_FP::OnFire()
{
	if (CachedPawnFP.IsValid())
	{
		APistolWeapon* Weapon = CachedPawnFP->GetWeapon();
		if (Weapon)
		{
			Weapon->StartFire();
		}
	}
}

void APistolPlayerController_FP::OnReload()
{
	if (CachedPawnFP.IsValid())
	{
		APistolWeapon* Weapon = CachedPawnFP->GetWeapon();
		if (Weapon)
		{
			Weapon->ReloadWeapon();
		}
	}
}

void APistolPlayerController_FP::OnMoveUp(const float Val)
{
	if (CachedPawnFP.IsValid())
	{
		UCameraComponent* PawnCamera = CachedPawnFP->GetCamera();

		if (PawnCamera)
		{
			const FVector OldLocation = PawnCamera->GetRelativeLocation();
			float Deviation = OldLocation.Z + ControlMovementSpeed * Val * GetWorld()->GetDeltaSeconds();

			// Normalize deviation
			Deviation = Deviation < MinUpDeviation ? MinUpDeviation : Deviation > MaxUpDeviation ? MaxUpDeviation : Deviation;
			const FVector NewLocation(OldLocation.X, OldLocation.Y, Deviation);
			PawnCamera->SetRelativeLocation(NewLocation);
		}
	}
}

void APistolPlayerController_FP::OnMoveRight(const float Val)
{
	if (CachedPawnFP.IsValid())
	{
		UCameraComponent* PawnCamera = CachedPawnFP->GetCamera();

		if (PawnCamera)
		{
			const FVector OldLocation = PawnCamera->GetRelativeLocation();
			float Deviation = OldLocation.Y + ControlMovementSpeed * Val * GetWorld()->GetDeltaSeconds();

			// Normalize deviation
			Deviation = abs(Deviation) > MaxRightDeviation ? Deviation > 0 ? MaxRightDeviation : MaxRightDeviation * -1 : Deviation;
			const FVector NewLocation(OldLocation.X, Deviation, OldLocation.Z);
			PawnCamera->SetRelativeLocation(NewLocation);
		}
	}
}
