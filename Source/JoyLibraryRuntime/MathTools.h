#pragma once

#include <CoreMinimal.h>
#include <UnrealMathUtility.h>
#include <Kismet/BlueprintFunctionLibrary.h>

#include "MathTools.generated.h"

#define R2D (180.f / PI)
#define D2R (PI / 180.f)

template<typename T>
JOYLIBRARYRUNTIME_API FORCEINLINE T Clamp01(T _value)
{
	return FMath::Clamp(_value, T(0), T(1));
}

template<typename T>
JOYLIBRARYRUNTIME_API FORCEINLINE bool IsNaN(T _value)
{
	FMath::IsNaN(_value);
}

template<>
JOYLIBRARYRUNTIME_API FORCEINLINE bool IsNaN(float _value)
{
	return FMath::IsNaN(_value);
}

template<>
JOYLIBRARYRUNTIME_API FORCEINLINE bool IsNaN(FQuat _value)
{
	return IsNaN(_value.X) || IsNaN(_value.Y) || IsNaN(_value.Z) || IsNaN(_value.W);
}

template<>
JOYLIBRARYRUNTIME_API FORCEINLINE bool IsNaN(FVector _value)
{
	return IsNaN(_value.X) || IsNaN(_value.Y) || IsNaN(_value.Z);
}

template<typename T>
JOYLIBRARYRUNTIME_API FORCEINLINE bool IsFinite(T _value)
{
	FMath::IsFinite(_value);
}

template<>
JOYLIBRARYRUNTIME_API FORCEINLINE bool IsFinite(float _value)
{
	return FMath::IsFinite(_value);
}

template<>
JOYLIBRARYRUNTIME_API FORCEINLINE bool IsFinite(FVector _value)
{
	return IsFinite(_value.X) && IsFinite(_value.Y) && IsFinite(_value.Z);
}

template<>
JOYLIBRARYRUNTIME_API FORCEINLINE bool IsFinite(FQuat _value)
{
	return IsFinite(_value.X) && IsFinite(_value.Y) && IsFinite(_value.Z) && IsFinite(_value.W);
}

JOYLIBRARYRUNTIME_API FORCEINLINE FVector ProjectPointOnPlane(const FVector& _point, const FPlane& _plane)
{
	return _point - _plane.GetSafeNormal() * _plane.PlaneDot(_point);
}

JOYLIBRARYRUNTIME_API FORCEINLINE FVector ProjectPointOnLine(const FVector& _point, const FVector& _lineA, const FVector& _lineB)
{
	FVector line = _lineB - _lineA;
	return _lineA + FVector::DotProduct(_point - _lineA, line) / FVector::DotProduct(line, line) * line;
}

JOYLIBRARYRUNTIME_API FORCEINLINE bool IsPointLeftFromLine(const FVector& _point, const FVector& _lineA, const FVector& _lineB, const FVector& _up = FVector::UpVector)
{
	return FVector::DotProduct(FVector::CrossProduct(_point - _lineA, _lineB - _lineA), _up) > 0.f;
}

/* In Radians */
JOYLIBRARYRUNTIME_API FORCEINLINE float AngleBetween(const FVector& _vectorA, const FVector& _vectorB)
{
	return FMath::Acos(FVector::DotProduct(_vectorA.GetSafeNormal(), _vectorB.GetSafeNormal()));
}

/* In Radians, Positive if B is right from A, negative if B is left A*/
JOYLIBRARYRUNTIME_API FORCEINLINE float SignedAngleBetween(const FVector& _vectorA, const FVector& _vectorB, const FVector& _up = FVector::UpVector)
{
	return AngleBetween(_vectorA, _vectorB) * (IsPointLeftFromLine(_vectorA, FVector::ZeroVector, _vectorB, _up) ? -1.f : 1.f);
}

JOYLIBRARYRUNTIME_API FORCEINLINE FVector GetYZ(const FVector& _vector)
{
	FVector v = _vector;
	v.X = 0.f;
	return v;
}

JOYLIBRARYRUNTIME_API FORCEINLINE FVector2D GetYZ2D(const FVector& _vector)
{
	return FVector2D(_vector.Y, _vector.Z);
}

JOYLIBRARYRUNTIME_API FORCEINLINE FVector GetXZ(const FVector& _vector)
{
	FVector v = _vector;
	v.Y = 0.f;
	return v;
}

JOYLIBRARYRUNTIME_API FORCEINLINE FVector2D GetXZ2D(const FVector& _vector)
{
	return FVector2D(_vector.X, _vector.Z);
}

JOYLIBRARYRUNTIME_API FORCEINLINE FVector GetXY(const FVector& _vector)
{
	FVector v = _vector;
	v.Z = 0.f;
	return v;
}

JOYLIBRARYRUNTIME_API FORCEINLINE FVector2D GetXY2D(const FVector& _vector)
{
	return FVector2D(_vector.X, _vector.Y);

}

template <typename T>
JOYLIBRARYRUNTIME_API FORCEINLINE T TimeIndependentLerp(T _base, T _target, float _ratio, float _dt)
{
	return FMath::Lerp(_base, _target, (1.f - FMath::Pow(_ratio, _dt)));
}

template <typename T>
JOYLIBRARYRUNTIME_API FORCEINLINE bool LineSphereIntersection(const T& _lineStart, const T& _lineEnd, const T& _sphereCenter, float _sphereRadius, float* _outT1 = nullptr, float* _outT2 = nullptr)
{
	T d = _lineEnd - _lineStart; // (Direction vector of ray, from start to end)
	T f = _lineStart - _sphereCenter; // (Vector from center sphere to ray start)

	float a = T::DotProduct(d, d);
	float b = 2 * T::DotProduct(f, d);
	float c = T::DotProduct(f, f) - _sphereRadius * _sphereRadius;

	float discriminant = b * b - 4 * a*c;
	if (discriminant < 0)
	{
		return false;
	}
	else
	{
		// ray didn't totally miss sphere,
		// so there is a solution to
		// the equation.

		discriminant = FMath::Sqrt(discriminant);

		// either solution may be on or off the ray so need to test both
		// t1 is always the smaller value, because BOTH discriminant and
		// a are nonnegative.
		float t1 = (-b - discriminant) / (2 * a);
		float t2 = (-b + discriminant) / (2 * a);

		if (_outT1) *_outT1 = t1;
		if (_outT2) *_outT2 = t2;

		return true;
	}
}

template <typename T>
JOYLIBRARYRUNTIME_API FORCEINLINE bool SegmentSphereIntersection(const T& _segmentStart, const T& _segmentEnd, const T& _sphereCenter, float _sphereRadius, float* _outT1 = nullptr, float* _outT2 = nullptr)
{
	float t1;
	float t2;
	if (!LineSphereIntersection(_segmentStart, _segmentEnd, _sphereCenter, _sphereRadius, &t1, &t2))
		return false;

	if (_outT1) *_outT1 = t1;
	if (_outT2) *_outT2 = t2;
	// 3x HIT cases:
		//          -o->             --|-->  |            |  --|->
		// Impale(t1 hit,t2 hit), Poke(t1 hit,t2>1), ExitWound(t1<0, t2 hit), 

		// 3x MISS cases:
		//       ->  o                     o ->              | -> |
		// FallShort (t1>1,t2>1), Past (t1<0,t2<0), CompletelyInside(t1<0, t2>1)

	if (t1 >= 0 && t1 <= 1)
	{
		// t1 is the intersection, and it's closer than t2
		// (since t1 uses -b - discriminant)
		// Impale, Poke
		return true;
	}

	// here t1 didn't intersect so we are either started
	// inside the sphere or completely past it
	if (t2 >= 0 && t2 <= 1)
	{
		// ExitWound
		return true;
	}

	// no intn: FallShort, Past, CompletelyInside
	return false;
}

JOYLIBRARYRUNTIME_API bool IsPointInSphere(const FVector _sphereCenter, const float _sphereRadius, const FVector _pointToTest);

JOYLIBRARYRUNTIME_API bool IsPointInCone(const FVector _origin, const FVector _direction, const float _angleInDegree, const float _distance, const FVector _pointToTest);

JOYLIBRARYRUNTIME_API void GetBoxVertices(FBox _box, FVector *_result);

JOYLIBRARYRUNTIME_API FVector GetRandomPointAroundLocationInTorus(FVector _location, FVector _direction, float _innerRadius, float _outterRadius);

JOYLIBRARYRUNTIME_API FVector GetPredictedShootLocation(FVector _fromLocation, FVector _targetLocation, FVector _targetVelocity, float _projectileSpeed);


/**
 *
 */
UCLASS()
class JOYLIBRARYRUNTIME_API UMathTools : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Joy|Math") static FRotator QuatToRotator(const FQuat& _quat);
	UFUNCTION(BlueprintPure, Category = "Joy|Math") static FQuat RotatorToQuat(const FRotator& _rotator);

	UFUNCTION(BlueprintPure, Category = "Joy|Math") static bool IsNearlyZero(float _value, float _tolerance = 0.00001f);
	UFUNCTION(BlueprintPure, Category = "Joy|Math") static bool LineSphereIntersection(const FVector& _lineStart, const FVector& _lineEnd, const FVector& _sphereCenter, float _sphereRadius, float& _outT1, float& _outT2);
	UFUNCTION(BlueprintPure, Category = "Joy|Math") static bool SegmentSphereIntersection(const FVector& _segmentStart, const FVector& _segmentEnd, const FVector& _sphereCenter, float _sphereRadius, float& _outT1, float& _outT2);
	UFUNCTION(BlueprintPure, Category = "Joy|Math") static bool LineSphereIntersection2D(const FVector2D& _lineStart, const FVector2D& _lineEnd, const FVector2D& _sphereCenter, float _sphereRadius, float& _outT1, float& _outT2);
	UFUNCTION(BlueprintPure, Category = "Joy|Math") static bool SegmentSphereIntersection2D(const FVector2D& _segmentStart, const FVector2D& _segmentEnd, const FVector2D& _sphereCenter, float _sphereRadius, float& _outT1, float& _outT2);
};


