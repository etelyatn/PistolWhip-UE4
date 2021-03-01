// 2021 github.com/EugeneTel/PistolWhip-UE4

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerState.h"
#include "PistolPlayerState.generated.h"

struct FEnemyHit;
class APistolPlayerState;
class APistolEnemyPawn;

DECLARE_MULTICAST_DELEGATE_FourParams(FOnPlayerDamaged, APistolPlayerState*, float, AController*, AActor*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTotalScoreUpdated, int32)
DECLARE_MULTICAST_DELEGATE_OneParam(FOneEnemyHit, APistolEnemyPawn*)

/**
 * 
 */
UCLASS()
class PISTOLWHIP_API APistolPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	APistolPlayerState();

	/** total player score */
	UPROPERTY(BlueprintReadOnly, Category = Scoring)
	float TotalScore;

	/** Event when enemy hit */
	FOneEnemyHit OnEnemyHit;

	/** Event when the player receives damage */
	FOnPlayerDamaged OnPlayerDamaged;

	/** Event when we're updating total score */
	FOnTotalScoreUpdated OnTotalScoreUpdated;

	void NotifyPlayerDamage(float Damage, AController* EventInstigator, AActor* DamageCauser);
	void NotifyPlayerDeath();

	/** Notify on enemy hit */
	void NotifyEnemyHit(FEnemyHit& EnemyHit);

	/** Calculate enemy damage score and display the result in UI and the enemy widget */
	void CalculateEnemyDamageScore(FEnemyHit& EnemyHit);
};
