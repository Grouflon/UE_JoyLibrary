#pragma once

#include <DrawDebugHelpers.h>

class UCanvas;
class UCurveFloat;

#define DRAW_POINT(position, color, thickness) DrawDebugPoint(GetWorld(), position, thickness, color)
#define DRAW_LINE(start, end, color, thickness) DrawDebugLine(GetWorld(), start, end, color, false, -1.f, 0.f, thickness)
#define DRAW_SPHERE(position, radius, color, thickness) DrawDebugSphere(GetWorld(), position, radius, 16, color, false, -1.f, 0, thickness)
#define DRAW_CIRCLE(position, radius, rotationQuat, color, thickness) DrawCircle(GetWorld(), position, rotationQuat.GetForwardVector(), rotationQuat.GetRightVector(), color, radius, 16, false, -1.f, 0, thickness)

#define DRAW_VECTOR(start, vector, color, thickness) {\
	FVector vectorNormal = (vector).GetSafeNormal();\
	float coneLength = FMath::Min(2.0f, (vector).Size());\
	float coneAngle = PI * 0.2f;\
	DrawDebugCone(GetWorld(), (start) + (vector), -vectorNormal, coneLength / FMath::Cos(coneAngle), coneAngle, coneAngle, 16, color, false, -1.f, 0, thickness);\
	DRAW_LINE(start, (start) + (vector), color, thickness);\
}

#define DRAW_TEXT(position, color, fmt, ...) \
{\
	DrawDebugString(GetWorld(), position, *FString::Printf(TEXT(fmt), __VA_ARGS__), nullptr, color, 0.0f, true);\
}

#define DRAW_PLANE(referencePoint, plane, size, color, thickness) \
{\
	FVector __closestPoint = ProjectPointOnPlane(referencePoint, plane); \
	DrawDebugBox(GetWorld(), __closestPoint, FVector(0.f, size, size), FRotationMatrix::MakeFromXZ(plane, FVector::UpVector).ToQuat(), color, false, -1.f, 0, thickness); \
	DRAW_VECTOR(__closestPoint, plane * (size), color, thickness); \
}

#define DRAW_CAPSULE(start, end, radius, color, thickness) \
{\
	FVector capsuleLength = (end) - (start);\
	FVector center = start + (capsuleLength * .5f);\
	FQuat rotation = FRotationMatrix::MakeFromZ(capsuleLength).ToQuat();\
	DrawDebugCapsule(GetWorld(), center, capsuleLength.Size() * .5f + (radius), radius, rotation, color, false, -1.f, 0, thickness);\
}

#define DRAW_ARROW(start, end, size, color, thickness)\
{\
	DrawDebugDirectionalArrow(GetWorld(), start, end, size, color, false, 0.f, 1, thickness);\
}

JOYLIBRARY_API void DrawDebug2DCone(const UWorld* _world, const FVector& _location, const FQuat& _rotation, float _halfHangleDeg, float _length, const FColor& _color, uint32 _samplingSteps = 32);

JOYLIBRARY_API void DrawDebugCanvasString(UCanvas* _canvas, FVector2D _location, const FString& _string, FColor _color);
JOYLIBRARY_API void DrawDebugCanvasCurve(UCanvas* _canvas, UCurveFloat* _curve, FBox2D _location, float _curveXMin, float _curveXMax, float _curveValue, const FString& _curveName, FColor _curveColor = FColor::Red, FColor _valueColor = FColor::Yellow, uint32 _samplesCount = 64);