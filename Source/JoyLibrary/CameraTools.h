#pragma once

class UCameraComponent;

#include <CoreMinimal.h>

JOYLIBRARY_API bool WorldToCameraNormalizedSpace(const FVector& _worldLocation, UCameraComponent* _camera, FVector2D* _outCameraNormalizedSpace, bool _constrained = true);
JOYLIBRARY_API void CameraNormalizedSpaceToWorld(const FVector2D& _cameraNormalizedLocation, UCameraComponent* _camera, FVector* _outWorldLocation, FVector* _outWorldDirection, bool _constrained = true);
