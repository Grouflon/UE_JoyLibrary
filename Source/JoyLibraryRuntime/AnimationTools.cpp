#include "AnimationTools.h"

#include <Log.h>
#include <Assert.h>

#include <Animation/AnimationAsset.h>
#include <Animation/AnimInstance.h>
#include <Animation/AnimSequence.h>
#include <Animation/AnimSingleNodeInstance.h>
#include <Animation/AnimStateMachineTypes.h>
#include <Animation/BlendSpaceBase.h>
//#include <AnimationBlueprintLibrary.h>
#include <Components/SkeletalMeshComponent.h>

float GetAnimationStateRelevantAnimTime(UAnimInstance* _animInstance, FName _stateMachineName, FName _stateName)
{
	int32 machineIndex = _animInstance->GetStateMachineIndex(_stateMachineName);
	if (machineIndex == INDEX_NONE)
	{
		LOGF_WARNING(TEXT("Unknown State Machine \"%s\"."), *_stateMachineName.ToString());
		return 0.f;
	}
	const FBakedAnimationStateMachine* machinePtr = _animInstance->GetStateMachineInstanceDesc(_stateMachineName);
	JOY_ASSERT(machinePtr);
	int32 stateIndex = machinePtr->FindStateIndex(_stateName);
	if (stateIndex == INDEX_NONE)
	{
		LOGF_WARNING(TEXT("Unknown State \"%s\" in StateMachine \"%s\"."), *_stateMachineName.ToString(), *_stateName.ToString());
		return 0.f;
	}

	return _animInstance->GetRelevantAnimTime(machineIndex, stateIndex);
}

float GetAnimationStateRelevantAnimTimeRemaining(UAnimInstance* _animInstance, FName _stateMachineName, FName _stateName)
{
	int32 machineIndex = _animInstance->GetStateMachineIndex(_stateMachineName);
	if (machineIndex == INDEX_NONE)
	{
		LOGF_WARNING(TEXT("Unknown State Machine \"%s\"."), *_stateMachineName.ToString());
		return 0.f;
	}
	const FBakedAnimationStateMachine* machinePtr = _animInstance->GetStateMachineInstanceDesc(_stateMachineName);
	JOY_ASSERT(machinePtr);
	int32 stateIndex = machinePtr->FindStateIndex(_stateName);
	if (stateIndex == INDEX_NONE)
	{
		LOGF_WARNING(TEXT("Unknown State \"%s\" in StateMachine \"%s\"."), *_stateMachineName.ToString(), *_stateName.ToString());
		return 0.f;
	}

	return _animInstance->GetRelevantAnimTimeRemaining(machineIndex, stateIndex);
}

float GetAnimationStateRelevantAnimTimeRemainingFraction(UAnimInstance* _animInstance, FName _stateMachineName, FName _stateName)
{
	int32 machineIndex = _animInstance->GetStateMachineIndex(_stateMachineName);
	if (machineIndex == INDEX_NONE)
	{
		LOGF_WARNING(TEXT("Unknown State Machine \"%s\"."), *_stateMachineName.ToString());
		return 0.f;
	}
	const FBakedAnimationStateMachine* machinePtr = _animInstance->GetStateMachineInstanceDesc(_stateMachineName);
	JOY_ASSERT(machinePtr);
	int32 stateIndex = machinePtr->FindStateIndex(_stateName);
	if (stateIndex == INDEX_NONE)
	{
		LOGF_WARNING(TEXT("Unknown State \"%s\" in StateMachine \"%s\"."), *_stateMachineName.ToString(), *_stateName.ToString());
		return 0.f;
	}

	return _animInstance->GetRelevantAnimTimeRemainingFraction(machineIndex, stateIndex);
}

const FAnimNotifyEvent* FindAnimationNotify(const UAnimSequence* _animation, FName _notifyName)
{
	TArray<FAnimNotifyEventReference> notifyReferences;
	_animation->GetAnimNotifies(0.f, const_cast<UAnimSequence*>(_animation)->GetPlayLength(), false, notifyReferences);
	for (const FAnimNotifyEventReference& ref : notifyReferences)
	{
		const FAnimNotifyEvent* evt = ref.GetNotify();
		if (evt->NotifyName == _notifyName)
		{
			return evt;
		}
	}
	return nullptr;
}

float FindAnimationNotifyTriggerTime(const UAnimSequence* _animation, FName _notifyName)
{
	const FAnimNotifyEvent* e = FindAnimationNotify(_animation, _notifyName);
	return e ? e->GetTriggerTime() : 0.f;
}

float FindStateMachineTransitionCrossFadeTime(UAnimInstance* _animInstance, FName _stateMachineName, FName _fromStateName, FName _toStateName)
{
	int32 machineIndex = _animInstance->GetStateMachineIndex(_stateMachineName);
	if (machineIndex == INDEX_NONE)
	{
		LOGF_WARNING(TEXT("Unknown State Machine \"%s\"."), *_stateMachineName.ToString());
		return 0.f;
	}
	const FBakedAnimationStateMachine* machinePtr = _animInstance->GetStateMachineInstanceDesc(_stateMachineName);
	JOY_ASSERT(machinePtr);
	int32 fromStateIndex = machinePtr->FindStateIndex(_fromStateName);
	if (fromStateIndex == INDEX_NONE)
	{
		LOGF_WARNING(TEXT("Unknown State \"%s\" in StateMachine \"%s\"."), *_stateMachineName.ToString(), *_fromStateName.ToString());
		return 0.f;
	}
	int32 toStateIndex = machinePtr->FindStateIndex(_toStateName);
	if (toStateIndex == INDEX_NONE)
	{
		LOGF_WARNING(TEXT("Unknown State \"%s\" in StateMachine \"%s\"."), *_stateMachineName.ToString(), *_toStateName.ToString());
		return 0.f;
	}

	int32 transitionIndex = machinePtr->FindTransitionIndex(fromStateIndex, toStateIndex);
	if (transitionIndex == INDEX_NONE)
	{
		LOGF_WARNING(TEXT("Unknown Transition from state \"%s\" to state \"%s\" in StateMachine \"%s\"."), *_fromStateName.ToString() , *_toStateName.ToString(), *_stateMachineName.ToString());
		return 0.f;
	}
	return machinePtr->Transitions[transitionIndex].CrossfadeDuration;
}

bool GetBoneChain(const USkeletalMeshComponent* _skeletalMesh, const FName& _firstBone, const FName& _lastBone, TArray<FName>& _outChain)
{
	return GetBoneChain(_skeletalMesh->SkeletalMesh->RefSkeleton, _firstBone, _lastBone, _outChain);
}

bool GetBoneChain(const USkeleton* _skeleton, const FName& _firstBone, const FName& _lastBone, TArray<FName>& _outChain)
{
	return GetBoneChain(_skeleton->GetReferenceSkeleton(), _firstBone, _lastBone, _outChain);
}

bool GetBoneChain(const FReferenceSkeleton& _referenceSkeleton, const FName& _firstBone, const FName& _lastBone, TArray<FName>& _outChain)
{
	int32 firstBoneIndex = _referenceSkeleton.FindRawBoneIndex(_firstBone);
	if (firstBoneIndex == INDEX_NONE)
		return false;

	int32 lastBoneIndex = _referenceSkeleton.FindRawBoneIndex(_lastBone);
	if (lastBoneIndex == INDEX_NONE)
		return false;

	if (_firstBone == _lastBone)
	{
		_outChain.Add(_firstBone);
		return true;
	}

	TArray<int32> firstBoneChain;
	firstBoneChain.Add(firstBoneIndex);
	{
		int32 parentBone = _referenceSkeleton.GetParentIndex(firstBoneChain.Last());
		while (parentBone != INDEX_NONE)
		{
			firstBoneChain.Add(parentBone);

			parentBone = _referenceSkeleton.GetParentIndex(firstBoneChain.Last());
		}
	}

	TArray<int32> lastBoneChain;
	lastBoneChain.Add(lastBoneIndex);
	{
		int32 parentBone = _referenceSkeleton.GetParentIndex(lastBoneChain.Last());
		while (parentBone != INDEX_NONE)
		{
			int32 index;
			if (firstBoneChain.Find(parentBone, index))
			{
				for (int32 i = 0; i <= index; ++i)
				{
					_outChain.Add(_referenceSkeleton.GetBoneName(firstBoneChain[i]));
				}

				for (int32 boneIndex : lastBoneChain)
				{
					_outChain.Add(_referenceSkeleton.GetBoneName(boneIndex));
				}
				return true;
			}

			lastBoneChain.EmplaceAt(0, parentBone);

			parentBone = _referenceSkeleton.GetParentIndex(lastBoneChain[0]);
		}
	}

	return false;
}

UAnimationAsset* UAnimationTools::GetAnimation(USkeletalMeshComponent* _skeletalMeshComponent)
{
	JOY_ASSERT(_skeletalMeshComponent);
	return _skeletalMeshComponent->GetSingleNodeInstance() ? _skeletalMeshComponent->GetSingleNodeInstance()->GetAnimationAsset() : nullptr;
}

FTransform UAnimationTools::ConsumeRootMotion(USkeletalMeshComponent* _skeletalMeshComponent)
{
	JOY_ASSERT(_skeletalMeshComponent);
	return _skeletalMeshComponent->ConsumeRootMotion().GetRootMotionTransform();
}

bool UAnimationTools::GetBoneChain(const USkeletalMeshComponent* _skeletalMesh, const FName& _firstBone, const FName& _lastBone, TArray<FName>& _outChain)
{
	return ::GetBoneChain(_skeletalMesh, _firstBone, _lastBone, _outChain);
}

FTransform UAnimationTools::ExtractRootMotionFromRange(const UAnimSequence* _animationSequence, float _startTrackPosition, float _endTrackPosition)
{
	JOY_EXITCONDITION_RET(_animationSequence == nullptr, FTransform::Identity, TEXT("Null Anim Sequence"));

	return _animationSequence->ExtractRootMotionFromRange(_startTrackPosition, _endTrackPosition);
}

FTransform UAnimationTools::ExtractRootTrackTransform(const UAnimSequence* _animationSequence, float _pos)
{
	JOY_EXITCONDITION_RET(_animationSequence == nullptr, FTransform::Identity, TEXT("Null Anim Sequence"));

	return _animationSequence->ExtractRootTrackTransform(_pos, NULL);
}


float UAnimationTools::GetPlayLength(const UAnimSequence* _animationSequence)
{
	JOY_EXITCONDITION_RET(_animationSequence == nullptr, 0.f, TEXT("Null Anim Sequence"));

	return const_cast<UAnimSequence*>(_animationSequence)->GetPlayLength(); // NOTE(Remi|2019/04/03): Unreal design of not putting this function const is fucked up and dumb
}

void UAnimationTools::RefreshAnimationCache(UAnimSequence* _animationSequence)
{
#if WITH_EDITOR
	FPropertyChangedEvent emptyPropertyUpdateStruct(nullptr);
	_animationSequence->PostEditChangeProperty(emptyPropertyUpdateStruct);
	_animationSequence->PostProcessSequence();
	_animationSequence->RefreshCurveData();
#endif
	_animationSequence->RefreshCacheData();
}
