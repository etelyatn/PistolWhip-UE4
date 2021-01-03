// 2021 github.com/EugeneTel/PistolWhip-UE4


#include "Weapon/PistolWeapon.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"

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

void APistolWeapon::PlayWeaponAnimation(UAnimationAsset* Animation)
{
	if (IsValid(MeshComponent) && IsValid(Animation))
	{
		MeshComponent->PlayAnimation(Animation, false);
	}
}

void APistolWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APistolWeapon::StartFire()
{
	PlayWeaponAnimation(FireAnim);
}

