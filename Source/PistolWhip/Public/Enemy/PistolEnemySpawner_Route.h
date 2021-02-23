// 2021 github.com/EugeneTel/PistolWhip-UE4

#pragma once

#include "CoreMinimal.h"
#include "Enemy/PistolEnemySpawner.h"
#include "PistolEnemySpawner_Route.generated.h"

/**
 * Enemy spawner with spline route
 */
UCLASS()
class PISTOLWHIP_API APistolEnemySpawner_Route : public APistolEnemySpawner
{
	GENERATED_BODY()

public:

	APistolEnemySpawner_Route();
	virtual void BeginPlay() override;

protected:

	/** a spline component for enemy movement */
	UPROPERTY(Category=EnemySpawner, VisibleAnywhere)
	USplineComponent* SplineComponent;
};
