// 2021 github.com/EugeneTel/PistolWhip-UE4

#include "Enemy/PistolEnemyAIController.h"

#include "Log.h"
#include "Enemy/PistolEnemyPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/PistolPlayerPawn.h"

class APistolPlayerPawn;

void APistolEnemyAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	CachedEnemyPawn = Cast<APistolEnemyPawn>(InPawn);
}

void APistolEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_Firing, this, &APistolEnemyAIController::Fire, FiringDelay, true);

	CachedPlayerPawn = Cast<APistolPlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

void APistolEnemyAIController::UpdateControlRotation(float DeltaTime, bool bUpdatePawn)
{
	if (!CachedPlayerPawn.IsValid() || !CachedEnemyPawn.IsValid())
	{
		return;
	}
	
	const FRotator NewControlRotation = UKismetMathLibrary::FindLookAtRotation(CachedEnemyPawn->GetActorLocation(), CachedPlayerPawn->GetActorLocation());
	SetControlRotation(NewControlRotation);

	if (bUpdatePawn)
	{
		CachedEnemyPawn->FaceRotation(NewControlRotation);
	}	
}

void APistolEnemyAIController::Fire()
{
	if (!CachedEnemyPawn.IsValid() || !CachedPlayerPawn.IsValid())
	{
		return;
	}

	APistolWeapon* Weapon = CachedEnemyPawn->GetWeapon();
	if (Weapon && CachedEnemyPawn->IsAlive())
	{
		Weapon->SetGoalLocation(CachedPlayerPawn->GetHeadLocation());
		Weapon->StartFire();
	}

}
