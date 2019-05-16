#include "SplineTools.h"

#include <Components/SplineComponent.h>


float FindDistanceAlongSplineClosestToWorldLocation(const USplineComponent* _spline, const FVector& _worldLocation, int32 _distanceSolverIterations)
{
	const float ClosestInputKey = _spline->FindInputKeyClosestToWorldLocation(_worldLocation);
	const int32 PreviousPoint = FMath::TruncToInt(ClosestInputKey);

	// Lerp between the previous and the next spline points
	float Distance = _spline->GetDistanceAlongSplineAtSplinePoint(PreviousPoint);
	Distance += (ClosestInputKey - PreviousPoint) * (_spline->GetDistanceAlongSplineAtSplinePoint(PreviousPoint + 1) - Distance);

	// The linear approximation is not enough. So here is a kinda numerical approximation.
	for (int32 i = 0; i < _distanceSolverIterations; ++i)
	{
		const float InputKeyAtDistance = _spline->SplineCurves.ReparamTable.Eval(Distance, 0.0f);
		// The euclidean distance between the current calculated distance and the real closest point
		const float Delta = (_spline->GetLocationAtSplineInputKey(InputKeyAtDistance, ESplineCoordinateSpace::World) - _spline->GetLocationAtSplineInputKey(ClosestInputKey, ESplineCoordinateSpace::World)).Size();
		if (InputKeyAtDistance < ClosestInputKey)
		{
			Distance += Delta;
		}
		else if (InputKeyAtDistance > ClosestInputKey)
		{
			Distance -= Delta;
		}
		else
		{
			break;
		}
	}
	return FMath::Clamp(Distance, 0.0f, _spline->GetSplineLength());
}
