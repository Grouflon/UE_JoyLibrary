#include "Math.h"

#include <Kismet/KismetMathLibrary.h>

bool IsPointInSphere(const FVector _sphereCenter, const float _sphereRadius, const FVector _pointToTest)
{
	return (FVector(_sphereCenter - _pointToTest).SizeSquared() <= FMath::Square<float>(_sphereRadius));
}

bool IsPointInCone(const FVector _origin, const FVector _direction, const float _angleInDegree, const float _distance, const FVector _pointToTest)
{
	if (IsPointInSphere(_origin, _distance, _pointToTest))
	{
		float angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(UKismetMathLibrary::GetDirectionUnitVector(_origin,_pointToTest),_direction)));

		if (angle <= _angleInDegree)
			return (true);
	}

	return (false);
}
