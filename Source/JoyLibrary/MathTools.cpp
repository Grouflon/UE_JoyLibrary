#include "MathTools.h"

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

void GetBoxVertices(FBox _box, FVector *_result)
{
	// Get Box Min Max Point
	FVector minBox = _box.GetExtrema(0);
	FVector maxBox = _box.GetExtrema(1);

	// Lower Points
	_result[0] = FVector(maxBox.X, maxBox.Y, minBox.Z);
	_result[1] = FVector(maxBox.X, minBox.Y, minBox.Z);

	_result[2] = FVector(minBox.X, maxBox.Y, minBox.Z);
	_result[3] = FVector(minBox.X, minBox.Y, minBox.Z);

	// Upper Points
	_result[4] = FVector(maxBox.X, maxBox.Y, maxBox.Z);
	_result[5] = FVector(maxBox.X, minBox.Y, maxBox.Z);

	_result[6] = FVector(minBox.X, maxBox.Y, maxBox.Z);
	_result[7] = FVector(minBox.X, minBox.Y, maxBox.Z);
}

FVector GetRandomPointAroundLocationInTorus(FVector _location, FVector _direction, float _innerRadius, float _outterRadius)
{
	return (FVector::CrossProduct(UKismetMathLibrary::RandomUnitVector(), _direction) * _outterRadius).GetClampedToSize(_innerRadius, _outterRadius) + _location;
}

FVector GetPredictedShootLocation(FVector _fromLocation, FVector _targetLocation, FVector _targetVelocity, float _projectileSpeed)
{
	return (_targetVelocity * ((_fromLocation - _targetLocation).Size() / _projectileSpeed)) + _targetLocation;
}

