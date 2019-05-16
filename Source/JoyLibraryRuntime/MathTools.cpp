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

FRotator UMathTools::QuatToRotator(const FQuat& _quat)
{
	return _quat.Rotator();
}

FQuat UMathTools::RotatorToQuat(const FRotator& _rotator)
{
	return _rotator.Quaternion();
}

bool UMathTools::IsNearlyZero(float _value, float _tolerance /*= SMALL_NUMBER*/)
{
	return FMath::IsNearlyZero(_value, _tolerance);
}

bool UMathTools::LineSphereIntersection(const FVector& _lineStart, const FVector& _lineEnd, const FVector& _sphereCenter, float _sphereRadius, float& _outT1, float& _outT2)
{
	return ::LineSphereIntersection(_lineStart, _lineEnd, _sphereCenter, _sphereRadius, &_outT1, &_outT2);
}

bool UMathTools::SegmentSphereIntersection(const FVector& _segmentStart, const FVector& _segmentEnd, const FVector& _sphereCenter, float _sphereRadius, float& _outT1 /*= nullptr*/, float& _outT2 /*= nullptr*/)
{
	return ::SegmentSphereIntersection(_segmentStart, _segmentEnd, _sphereCenter, _sphereRadius, &_outT1, &_outT2);
}

bool UMathTools::LineSphereIntersection2D(const FVector2D& _lineStart, const FVector2D& _lineEnd, const FVector2D& _sphereCenter, float _sphereRadius, float& _outT1, float& _outT2)
{
	return ::LineSphereIntersection(_lineStart, _lineEnd, _sphereCenter, _sphereRadius, &_outT1, &_outT2);
}

bool UMathTools::SegmentSphereIntersection2D(const FVector2D& _segmentStart, const FVector2D& _segmentEnd, const FVector2D& _sphereCenter, float _sphereRadius, float& _outT1, float& _outT2)
{
	return ::SegmentSphereIntersection(_segmentStart, _segmentEnd, _sphereCenter, _sphereRadius, &_outT1, &_outT2);
}
