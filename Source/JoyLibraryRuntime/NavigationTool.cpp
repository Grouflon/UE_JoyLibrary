#include "NavigationTool.h"

// Unreal includes
#include <AIController.h>
#include <Log.h>
 
#include <Runtime/NavigationSystem/Public/NavigationSystem.h>
#include <Runtime/NavigationSystem/Public/NavigationPath.h>
#include <NavigationQueryFilter.h> 
#include <Navmesh/RecastNavMesh.h>

#include <Kismet/GameplayStatics.h> 

ARecastNavMesh* GetRecastNavMesh(const UObject* _worldContextObject, FString _navMeshName)
{
	ARecastNavMesh* selectedRecastNavMesh = nullptr;
	  
	TArray<AActor*> recastsNavMeshes;
	UGameplayStatics::GetAllActorsOfClass(_worldContextObject, ARecastNavMesh::StaticClass(), recastsNavMeshes);
	for (AActor* currentActor : recastsNavMeshes)
	{
		ARecastNavMesh* navMesh = Cast<ARecastNavMesh>(currentActor);
		if (navMesh->GetName().Contains(_navMeshName))
		{
#if JOY_ASSERT_ENABLED 
			JOY_ASSERT_MSG(!selectedRecastNavMesh, TEXT("Looks like you have multiple identical RecastNavMesh actor in your level"));
			selectedRecastNavMesh = navMesh;
#else
			selectedRecastNavMesh = navMesh;
			break;
#endif
		} 
	}
	if (selectedRecastNavMesh == nullptr) 
	{
		if (recastsNavMeshes.Num() > 0)
		{
			selectedRecastNavMesh = Cast<ARecastNavMesh>(recastsNavMeshes[0]);
		} 
	}

	return selectedRecastNavMesh;
}


ARecastNavMesh* UNavigationTool::GetRecastNavMesh(UObject* WorldContextObject, FString _navMeshName)
{
	return ::GetRecastNavMesh(WorldContextObject->GetWorld(), _navMeshName);
}

