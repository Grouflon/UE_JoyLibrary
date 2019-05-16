#include "Draw.h"

#include <Engine/Canvas.h>
#include <Curves/CurveFloat.h>
#include <Components/SphereComponent.h>
#include <Components/BoxComponent.h>
#include <Components/CapsuleComponent.h>

#include <MathTools.h>

void DrawDebugVector(const UWorld* _world, const FVector& _start, const FVector& _vector, const FColor& _color, bool _bPersistentLines /*= false*/, float _lifeTime /*= -1.f*/, uint8 _depthPriority /*= 0*/, float _thickness /*= 0*/)
{
	FVector vectorNormal = _vector.GetSafeNormal();
	float coneLength = FMath::Min(2.0f, _vector.Size());
	float coneAngle = PI * 0.2f;
	DrawDebugCone(_world, _start + _vector, -vectorNormal, coneLength / FMath::Cos(coneAngle), coneAngle, coneAngle, 16, _color, _bPersistentLines, _lifeTime, _depthPriority, _thickness);
	DrawDebugLine(_world, _start, _start + _vector, _color, _bPersistentLines, _lifeTime, _depthPriority, _thickness);
}

void DrawDebugPlane(const UWorld* _world, const FVector& _referencePoint, const FPlane& _plane, float _planeSize, const FColor& _color, bool _bPersistentLines /*= false*/, float _lifeTime /*= -1.f*/, uint8 _depthPriority /*= 0*/, float _thickness /*= 0*/)
{
	FVector closestPoint = ProjectPointOnPlane(_referencePoint, _plane);
	DrawDebugBox(_world, closestPoint, FVector(0.f, _planeSize, _planeSize), FRotationMatrix::MakeFromXZ(_plane, FVector::UpVector).ToQuat(), _color, _bPersistentLines, _lifeTime, _depthPriority, _thickness);
	DrawDebugVector(_world, closestPoint, _plane * _planeSize, _color, _bPersistentLines, _lifeTime, _depthPriority, _thickness);
}

void DrawDebugCapsule(const UWorld* _world, const FVector& _start, const FVector& _end, float _radius, const FColor& _color, bool _bPersistentLines /*= false*/, float _lifeTime /*= -1.f*/, uint8 _depthPriority /*= 0*/, float _thickness /*= 0*/)
{
	FVector capsuleLength = _end - _start;
	FVector center = _start + (capsuleLength * .5f);
	FQuat rotation = FRotationMatrix::MakeFromZ(capsuleLength).ToQuat();
	DrawDebugCapsule(_world, center, capsuleLength.Size() * .5f + _radius, _radius, rotation, _color, false, -1.f, 0, _thickness);
}

void DrawDebug2DCone(const UWorld* _world, const FVector& _location, const FQuat& _rotation, float _halfHangleDeg, float _length, const FColor& _color, uint32 _samplingSteps /*= 32*/)
{
	if (_samplingSteps <= 0)
		return;

	TArray<FVector> vertices;
	TArray<int32> indices;

	vertices.Add(_location);

	float step = _halfHangleDeg * 2.f / float(_samplingSteps);
	for (uint32 i = 0; i < _samplingSteps - 1; ++i)
	{
		float a1 = -_halfHangleDeg + i * step;
		float a2 = -_halfHangleDeg + (i + 1) * step;

		vertices.Add(_location + (_rotation.GetForwardVector() * _length).RotateAngleAxis(a1, _rotation.GetUpVector()));
		vertices.Add(_location + (_rotation.GetForwardVector() * _length).RotateAngleAxis(a2, _rotation.GetUpVector()));
		indices.Add(0);
		indices.Add(1 + i * 2);
		indices.Add(1 + i * 2 + 1);
	}

	DrawDebugMesh(_world, vertices, indices, _color);
}

void DrawPrimitiveComponent(const UPrimitiveComponent* _component, FColor _color, float _thickness)
{
	if (!_component)
		return;

	if (const UBoxComponent* box = Cast<UBoxComponent>(_component))
	{
		DrawDebugBox(box->GetWorld(), box->GetComponentLocation(), box->GetScaledBoxExtent(), box->GetComponentQuat(), _color, false, -1.f, 0, _thickness);
	}
	else if (const USphereComponent* sphere = Cast<USphereComponent>(_component))
	{
		DrawDebugSphere(sphere->GetWorld(), sphere->GetComponentLocation(), sphere->GetScaledSphereRadius(), 32, _color, false, -1.f, 0, _thickness);
	}
	else if (const UCapsuleComponent* capsule = Cast<UCapsuleComponent>(_component))
	{
		DrawDebugCapsule(capsule->GetWorld(), capsule->GetComponentLocation(), capsule->GetScaledCapsuleHalfHeight(), capsule->GetScaledCapsuleRadius(), capsule->GetComponentQuat(), _color, false, -1.f, 0, _thickness);
	}
}

void DrawDebugCanvasString(UCanvas* _canvas, FVector2D _location, const FString& _string, FColor _color)
{
	FDisplayDebugManager& DisplayDebugManager = _canvas->DisplayDebugManager;

	float* canvasYPtr = &DisplayDebugManager.GetYPosRef();
	float* canvasXPtr = canvasYPtr - 1; // DIRTY HACK
	float previousCanvasX = *canvasXPtr;
	float previousCanvasY = *canvasYPtr;

	*canvasXPtr = _location.X;
	*canvasYPtr = _location.Y;

	DisplayDebugManager.SetDrawColor(_color);
	DisplayDebugManager.DrawString(_string);

	*canvasXPtr = previousCanvasX;
	*canvasYPtr = previousCanvasY;
}

void DrawDebugCanvasCurve(UCanvas* _canvas, UCurveFloat* _curve, FBox2D _location, float _curveXMin, float _curveXMax, float _curveValue, const FString& _curveName, FColor _curveColor /*= FColor::Red*/, FColor _valueColor /*= FColor::Yellow*/, uint32 _samplesCount /*= 64*/)
{
	static const float s_charWidth = 5.f;
	static const float s_charHeight = 14.f;

	FVector2D curveCenter = _location.GetCenter();
	FVector2D curveExtent = _location.GetExtent();
	float curveYMin = TNumericLimits<float>::Max();
	float curveYMax = TNumericLimits<float>::Min();
	float curveLength = _curveXMax - _curveXMin;
	float curveStep = curveLength / float(_samplesCount);
	for (uint32 i = 0; i < _samplesCount - 1; ++i)
	{
		float y = _curve->GetFloatValue(_curveXMin + i * curveStep);
		curveYMin = FMath::Min(y, curveYMin);
		curveYMax = FMath::Max(y, curveYMax);
	}
	float curveHeight = curveYMax - curveYMin;

	DrawDebugCanvasString(_canvas, FVector2D(curveCenter.X - _curveName.Len() * s_charWidth * .5f, _location.Min.Y - s_charHeight), _curveName, _curveColor);
	DrawDebugCanvas2DLine(_canvas, _location.Min, _location.Min + FVector2D(0.f, curveExtent.Y * 2.f), _curveColor);
	DrawDebugCanvas2DLine(_canvas, _location.Max, _location.Max + -FVector2D(curveExtent.X * 2.f, 0.f), _curveColor);
	DrawDebugCanvasString(_canvas, FVector2D(_location.Min.X, _location.Max.Y), FString::Printf(TEXT("%.2f"), _curveXMin), _curveColor);
	DrawDebugCanvasString(_canvas, FVector2D(_location.Max.X, _location.Max.Y), FString::Printf(TEXT("%.2f"), _curveXMax), _curveColor);
	DrawDebugCanvasString(_canvas, FVector2D(_location.Min.X - 5.f * s_charWidth, _location.Min.Y - s_charHeight), FString::Printf(TEXT("%.2f"), curveYMax), _curveColor);
	DrawDebugCanvasString(_canvas, FVector2D(_location.Min.X - 5.f * s_charWidth, _location.Max.Y - s_charHeight), FString::Printf(TEXT("%.2f"), curveYMin), _curveColor);

	for (uint32 i = 0; i < _samplesCount - 1; ++i)
	{
		FVector2D firstPoint = FVector2D(_curveXMin + i * curveStep, _curve->GetFloatValue(_curveXMin + i * curveStep));
		FVector2D secondPoint = FVector2D(_curveXMin + (i + 1) * curveStep, _curve->GetFloatValue(_curveXMin + (i + 1) * curveStep));

		firstPoint.X = _location.Min.X + ((firstPoint.X - _curveXMin) / curveLength) * curveExtent.X * 2.f;
		firstPoint.Y = _location.Max.Y - ((firstPoint.Y - curveYMin) / curveHeight) * curveExtent.Y * 2.f;

		secondPoint.X = _location.Min.X + ((secondPoint.X - _curveXMin) / curveLength) * curveExtent.X * 2.f;
		secondPoint.Y = _location.Max.Y - ((secondPoint.Y - curveYMin) / curveHeight) * curveExtent.Y * 2.f;

		DrawDebugCanvas2DLine(_canvas, firstPoint, secondPoint, _curveColor);
	}


	if (_curveValue >= _curveXMin && _curveValue <= _curveXMax)
	{
		float yValue = _curve->GetFloatValue(_curveValue);
		FVector2D point = FVector2D(_curveValue, yValue);
		point.X = _location.Min.X + ((point.X - _curveXMin) / curveLength) * curveExtent.X * 2.f;
		point.Y = _location.Max.Y - ((point.Y - curveYMin) / curveHeight) * curveExtent.Y * 2.f;

		DrawDebugCanvas2DLine(_canvas, point, FVector2D(point.X, _location.Max.Y), _valueColor);
		DrawDebugCanvas2DLine(_canvas, point, FVector2D(_location.Min.X, point.Y), _valueColor);

		DrawDebugCanvasString(_canvas, FVector2D(point.X, _location.Max.Y), FString::Printf(TEXT("%.2f"), _curveValue), _valueColor);
		DrawDebugCanvasString(_canvas, FVector2D(_location.Min.X - 5 * s_charWidth, point.Y - s_charHeight), FString::Printf(TEXT("%.2f"), yValue), _valueColor);
	}
}
