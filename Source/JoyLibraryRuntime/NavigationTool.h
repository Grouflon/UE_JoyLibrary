#pragma once

class ARecastNavMesh;

// Unreal includes
#include <CoreMinimal.h>
#include <Kismet/BlueprintFunctionLibrary.h>

// Plugins includes

// Game includes
#include "NavigationTool.generated.h"

JOYLIBRARYRUNTIME_API ARecastNavMesh* GetRecastNavMesh(const UObject* _worldContextObject, FString _navMeshName);

UCLASS()
class JOYLIBRARYRUNTIME_API UNavigationTool : public UBlueprintFunctionLibrary
{ 
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject")) static ARecastNavMesh* GetRecastNavMesh(UObject* WorldContextObject, FString _navMeshName);
}; 

 