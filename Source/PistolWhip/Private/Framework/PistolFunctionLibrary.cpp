// 2021 github.com/EugeneTel/PistolWhip-UE4

#include "Framework/PistolFunctionLibrary.h"

float UPistolFunctionLibrary::AngleBetweenTwoVectors(const FVector VectorA, const FVector VectorB)
{
	// Dot Product
	const float DotProduct = FVector::DotProduct(VectorA, VectorB);
	const float LengthProduct = VectorA.Size() * VectorB.Size();

	return FMath::Acos(DotProduct / LengthProduct);
}
