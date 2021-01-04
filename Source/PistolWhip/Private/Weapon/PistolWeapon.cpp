// 2021 github.com/EugeneTel/PistolWhip-UE4


#include "Weapon/PistolWeapon.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "DrawDebugHelpers.h"
#include "PistolWhipTypes.h"

APistolWeapon::APistolWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetBoxExtent(FVector(15.0f, 2.0f, 6.0f));
	BoxComponent->SetupAttachment(RootComponent);

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
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
}

void APistolWeapon::BeginPlay()
{
	Super::BeginPlay();
}

bool APistolWeapon::CanFire() const
{
	return true;
}

void APistolWeapon::PlayWeaponAnimation(UAnimationAsset* Animation)
{
	if (IsValid(MeshComponent) && IsValid(Animation))
	{
		MeshComponent->PlayAnimation(Animation, false);
	}
}

FHitResult APistolWeapon::WeaponTrace(const FVector& TraceFrom, const FVector& TraceTo) const
{
	// Perform trace to retrieve hit info
	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(WeaponTrace), true, this);
	TraceParams.bReturnPhysicalMaterial = true;
	TraceParams.bDebugQuery = true;

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

void APistolWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APistolWeapon::StartFire()
{
	if (CanFire())
	{
		PlayWeaponAnimation(FireAnim);
		
		FireWeapon();
	}
}

void APistolWeapon::SetOwningPawn(APawn* NewOwner)
{
	if (OwnerPawn != NewOwner)
	{
		OwnerPawn = NewOwner;
		SetOwner(NewOwner);
		SetInstigator(NewOwner);
	}
}

