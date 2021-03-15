// 2021 github.com/EugeneTel/PistolWhip-UE4

#include "Framework/PistolFunctionLibrary.h"

float UPistolFunctionLibrary::AngleBetweenTwoVectors(const FVector VectorA, const FVector VectorB)
{
	// Dot Product
	const float DotProduct = FVector::DotProduct(VectorA, VectorB);
	const float LengthProduct = VectorA.Size() * VectorB.Size();

	return FMath::Acos(DotProduct / LengthProduct);
}

float UPistolFunctionLibrary::AngleBetweenTwoVectorsInDegrees(const FVector VectorA, const FVector VectorB)
{
	const float Angle = FMath::Acos(FVector::DotProduct(VectorA, VectorB));

	return FMath::RadiansToDegrees(Angle);
}

void UPistolFunctionLibrary::PistolRestartLevel(UObject* WorldContextObject)
{
	UWorld* const World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	if (World)
	{
		const FURL NewURL = GEngine->LastURLFromWorld(World);
				
		APlayerController* const PC = World->GetFirstPlayerController();
		if (PC)
		{
			PC->ClientTravel(NewURL.ToString(), TRAVEL_Relative);
		}
	}
}
