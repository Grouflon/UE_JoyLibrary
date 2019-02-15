#pragma once

class USkeletalMeshComponent;

#include <CoreMinimal.h>

JOYLIBRARY_API bool GetBoneChain(const USkeletalMeshComponent* _skeletalMesh, const FName& _firstBone, const FName& _lastBone, TArray<FName>& _outChain);
