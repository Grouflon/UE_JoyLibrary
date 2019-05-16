#pragma once

class UCameraComponent;

#include <CoreMinimal.h>
#include <Kismet/BlueprintFunctionLibrary.h>

#include "CameraTools.generated.h"

JOYLIBRARYRUNTIME_API bool WorldToCameraNormalizedSpace(const FVector& _worldLocation, UCameraComponent* _camera, FVector2D* _outCameraNormalizedSpace, bool _constrained = true);
JOYLIBRARYRUNTIME_API void CameraNormalizedSpaceToWorld(const FVector2D& _cameraNormalizedLocation, UCameraComponent* _camera, FVector* _outWorldLocation, FVector* _outWorldDirection, bool _constrained = true);

UCLASS()
class JOYLIBRARYRUNTIME_API UCameraTools : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Joy|CameraTools", meta = (WorldContext = "_worldContextObject")) static FVector GetCurrentViewLocation(UObject* _worldContextObject);


};
