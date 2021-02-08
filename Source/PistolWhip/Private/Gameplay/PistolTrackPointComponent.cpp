// 2021 github.com/EugeneTel/PistolWhip-UE4

#include "Gameplay/PistolTrackPointComponent.h"
#include "DrawDebugHelpers.h"
#include "Log.h"
#include "Enemy/PistolEnemySpawner.h"
#include "Player/PistolPlayerPawn.h"

UPistolTrackPointComponent::UPistolTrackPointComponent()
{
	LineThickness = 1.0;
	BoxExtent = FVector(10.0f);
	ShapeColor = FColor::Blue;
}

void UPistolTrackPointComponent::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UPistolTrackPointComponent::OnTrackPointBeginOverlap);
}

void UPistolTrackPointComponent::OnTrackPointBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APistolPlayerPawn* PlayerPawn = Cast<APistolPlayerPawn>(OtherActor);

	if (PlayerPawn)
	{
		ActivateItems();
	}
}

#if WITH_EDITOR
void UPistolTrackPointComponent::DrawLinesToItems()
{
	const UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	for (auto Item : Items)
	{
		if (Item)
		{
			AActor* ItemActor = Cast<AActor>(Item.GetObject());
			if (ItemActor)
			{
				DrawDebugLine(World, GetComponentLocation(), ItemActor->GetActorLocation() + FVector(0.0f, 0.0f, 10.0f), FColor::Green, false, -1, 0, 1.0f);
			}
		}
	}
}
#endif


void UPistolTrackPointComponent::ActivateItems()
{
	for (auto Item : Items)
	{
		if (Item.GetObject())
		{
			IPistolActivatableInterface::Execute_Activate(Item.GetObject());
		}		
	}
}

