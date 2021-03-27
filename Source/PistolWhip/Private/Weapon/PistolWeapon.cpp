// 2021 github.com/EugeneTel/PistolWhip-UE4


#include "Weapon/PistolWeapon.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "DrawDebugHelpers.h"
#include "PistolWhipTypes.h"
#include "Framework/PistolFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PistolHandController.h"
#include "Player/PlayerControllers/PistolPlayerController_FP.h"

APistolWeapon::APistolWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetBoxExtent(FVector(15.0f, 2.0f, 6.0f));
	BoxComponent->SetupAttachment(RootComponent);

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	MeshComponent->CastShadow = false;
	MeshComponent->SetupAttachment(RootComponent);

#if WITH_EDITORONLY_DATA
	ArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	if (ArrowComponent)
	{
		ArrowComponent->ArrowColor = FColor(150, 200, 255);
		ArrowComponent->SetupAttachment(RootComponent);
		ArrowComponent->bIsScreenSizeScaled = true;
	}
#endif

	// defaults
	bDebug = false;
	MuzzleName = FName("Muzzle");
	WeaponRange = 5000.0f;
}

void APistolWeapon::BeginPlay()
{
	Super::BeginPlay();

	// set full ammo in clip
	if (!HasInfiniteAmmo())
	{
		ReloadWeapon();
	}
}

bool APistolWeapon::CanFire() const
{
	return CurrentAmmoInClip > 0 || HasInfiniteAmmo();
}

void APistolWeapon::PlayWeaponAnimation(UAnimationAsset* Animation)
{
	if (IsValid(MeshComponent) && IsValid(Animation))
	{
		MeshComponent->PlayAnimation(Animation, false);
	}
}

void APistolWeapon::PlayHapticFeedback(UHapticFeedbackEffect_Base* HapticFeedback, float Scale)
{
	if (HapticFeedback)
	{
		OwnerHandController = Cast<APistolHandController>(GetAttachParentActor());
		if (OwnerHandController)
		{
			OwnerHandController->PlayHapticFeedback(HapticFeedback, Scale);
		}
	}
}

void APistolWeapon::PlayWeaponSound(USoundBase* SoundToPlay, const float Volume) const
{
	if (SoundToPlay)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundToPlay, Volume);
	}
}

FHitResult APistolWeapon::WeaponTrace(const FVector& TraceFrom, const FVector& TraceTo) const
{
	// Perform trace to retrieve hit info
	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(WeaponTrace), true, this);
	TraceParams.bReturnPhysicalMaterial = true;
	TraceParams.bDebugQuery = bDebug;

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceFrom, TraceTo, COLLISION_WEAPON_TRACE, TraceParams);

#if !UE_BUILD_SHIPPING
	// Draw debug line
	if (bDebug)
	{
		DrawDebugLine(GetWorld(), TraceFrom, TraceTo, FColor::Green, false, 3, 0.f, 0.5f);

		if (Hit.bBlockingHit)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 10.f, 16, FColor::Red,false,3.f,0,0.5f);
		}
	}
#endif

	return Hit;
}

FVector APistolWeapon::GetMuzzleLocation() const
{
	return MeshComponent->GetSocketLocation(MuzzleName);
}

FVector APistolWeapon::GetMuzzleDirection() const
{
	return MeshComponent->GetSocketRotation(MuzzleName).Vector();
}

bool APistolWeapon::HasInfiniteAmmo() const
{
	return WeaponData.bInfiniteAmmo;
}

int32 APistolWeapon::GetAmmoPerClip() const
{
	return WeaponData.AmmoPerClip;
}

int32 APistolWeapon::GetCurrentAmmoInClip() const
{
	return CurrentAmmoInClip;
}

FVector APistolWeapon::GetDamageStartLocation() const
{
	APistolPlayerController_FP* const PlayerController = EquippingPawn ? Cast<APistolPlayerController_FP>(EquippingPawn->Controller) : nullptr;
	FVector OutStartTrace;

	if (PlayerController)
	{
		// use player's camera
		FVector CamLoc;
		FRotator CamRot;
		PlayerController->GetPlayerViewPoint(CamLoc, CamRot);

		// Adjust trace so there is nothing blocking the ray between the camera and the pawn, and calculate distance from adjusted start
		OutStartTrace = CamLoc + CamRot.Vector() * ((GetInstigator()->GetActorLocation() - CamLoc) | CamRot.Vector());
	}
	else
	{
		OutStartTrace = GetMuzzleLocation();
	}

	return OutStartTrace;
}

FVector APistolWeapon::GetAdjustedAim() const
{
	APistolPlayerController_FP* const PlayerController = EquippingPawn ? Cast<APistolPlayerController_FP>(EquippingPawn->Controller) : nullptr;
	FVector FinalAim = FVector::ZeroVector;
	
	// If we have a FP player controller use it for the aim
	if (PlayerController)
	{
		FVector CamLoc;
		FRotator CamRot;
		PlayerController->GetPlayerViewPoint(CamLoc, CamRot);
		FinalAim = CamRot.Vector();
	}
	else
	{
		FinalAim = GetMuzzleDirection();
	}

	return FinalAim;
}

void APistolWeapon::UseAmmo()
{
	if (!HasInfiniteAmmo())
	{
		CurrentAmmoInClip--;
		NotifyAmmoUpdated(CurrentAmmoInClip);
	}
}

bool APistolWeapon::CanReload() const
{
	return ( CurrentAmmoInClip < WeaponData.AmmoPerClip) && !HasInfiniteAmmo();
}

void APistolWeapon::ReloadWeapon()
{
	if (CanReload())
	{
		CurrentAmmoInClip = GetAmmoPerClip();
		PlayHapticFeedback(ReloadHapticFeedback);
		PlayWeaponSound(ReloadSound);
		NotifyAmmoUpdated(CurrentAmmoInClip);
	}
}

void APistolWeapon::CheckAndHandleReload()
{
	if (CanReload())
	{
		const float UpAngle = UPistolFunctionLibrary::AngleBetweenTwoVectors(GetActorUpVector(), FVector::UpVector);
		if (UpAngle > 1.0f && GetActorRotation().Pitch < 0.0f)
		{
			ReloadWeapon();
		}
	}
}

void APistolWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasInfiniteAmmo())
	{
		CheckAndHandleReload();
	}
}

void APistolWeapon::StartFire()
{
	if (CanFire())
	{
		PlayWeaponAnimation(FireAnim);
		PlayHapticFeedback(FireHapticFeedback);
		
		FireWeapon();

		UseAmmo();
	}
	else
	{
		PlayHapticFeedback(FireHapticFeedback, 0.3f);
		PlayWeaponSound(NoAmmoSound);
	}
}

void APistolWeapon::EquippedBy(APistolBasePawn* InPawn)
{
	EquippingPawn = InPawn;
	SetInstigator(InPawn);
}

