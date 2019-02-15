#include "VisualTools.h"

#include <Components/SkeletalMeshComponent.h>

bool GetBoneChain(const USkeletalMeshComponent* _skeletalMesh, const FName& _firstBone, const FName& _lastBone, TArray<FName>& _outChain)
{
	if (_skeletalMesh->GetBoneIndex(_firstBone) == INDEX_NONE)
		return false;

	if (_skeletalMesh->GetBoneIndex(_lastBone) == INDEX_NONE)
		return false;

	if (_firstBone == _lastBone)
	{
		_outChain.Add(_firstBone);
		return true;
	}

	TArray<FName> firstBoneChain;
	firstBoneChain.Add(_firstBone);
	{
		FName parentBone = _skeletalMesh->GetParentBone(firstBoneChain.Last());
		while (!parentBone.IsNone())
		{
			firstBoneChain.Add(parentBone);

			parentBone = _skeletalMesh->GetParentBone(firstBoneChain.Last());
		}
	}
	
	TArray<FName> lastBoneChain;
	lastBoneChain.Add(_lastBone);
	{
		FName parentBone = _skeletalMesh->GetParentBone(lastBoneChain.Last());
		while (!parentBone.IsNone())
		{
			int32 index;
			if (firstBoneChain.Find(parentBone, index))
			{
				for (int32 i = 0; i <= index; ++i)
				{
					_outChain.Add(firstBoneChain[i]);
				}
				_outChain += lastBoneChain;
				return true;
			}

			lastBoneChain.EmplaceAt(0, parentBone);

			parentBone = _skeletalMesh->GetParentBone(lastBoneChain[0]);
		}
	}

	return false;
}
