#pragma once

class USplineComponent;

#include <CoreMinimal.h>

JOYLIBRARYRUNTIME_API float FindDistanceAlongSplineClosestToWorldLocation(const USplineComponent* _spline, const FVector& _worldLocation, int32 _distanceSolverIterations = 3);
