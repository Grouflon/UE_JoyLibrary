#pragma once

#include <CoreMinimal.h>

#include "AnimationTools.generated.h"

struct FReferenceSkeleton;
class UBlendSpaceBase;

JOYLIBRARYRUNTIME_API float GetAnimationStateRelevantAnimTime(UAnimInstance* _animInstance, FName _stateMachineName, FName _stateName);
JOYLIBRARYRUNTIME_API float GetAnimationStateRelevantAnimTimeRemaining(UAnimInstance* _animInstance, FName _stateMachineName, FName _stateName);
JOYLIBRARYRUNTIME_API float GetAnimationStateRelevantAnimTimeRemainingFraction(UAnimInstance* _animInstance, FName _stateMachineName, FName _stateName);
JOYLIBRARYRUNTIME_API const FAnimNotifyEvent* FindAnimationNotify(const UAnimSequence* _animation, FName _notifyName);
JOYLIBRARYRUNTIME_API float FindAnimationNotifyTriggerTime(const UAnimSequence* _animation, FName _notifyName);
JOYLIBRARYRUNTIME_API float FindStateMachineTransitionCrossFadeTime(UAnimInstance* _animInstance, FName _stateMachineName, FName _sourceStateName, FName _targetStateName);

JOYLIBRARYRUNTIME_API bool GetBoneChain(const USkeletalMeshComponent* _skeletalMesh, const FName& _firstBone, const FName& _lastBone, TArray<FName>& _outChain);
JOYLIBRARYRUNTIME_API bool GetBoneChain(const USkeleton* _skeleton, const FName& _firstBone, const FName& _lastBone, TArray<FName>& _outChain);
JOYLIBRARYRUNTIME_API bool GetBoneChain(const FReferenceSkeleton& _referenceSkeleton, const FName& _firstBone, const FName& _lastBone, TArray<FName>& _outChain);

/**
 *
 */
UCLASS()
class JOYLIBRARYRUNTIME_API UAnimationTools : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joy|Animation") static UAnimationAsset* GetAnimation(USkeletalMeshComponent* _skeletalMeshComponent);
	UFUNCTION(BlueprintCallable, Category = "Joy|Animation") static FTransform ConsumeRootMotion(USkeletalMeshComponent* _skeletalMeshComponent);

	UFUNCTION(BlueprintCallable, Category = "Joy|Animation") static bool GetBoneChain(const USkeletalMeshComponent* _skeletalMesh, const FName& _firstBone, const FName& _lastBone, TArray<FName>& _outChain);

	UFUNCTION(BlueprintCallable, Category = "Joy|Animation") static FTransform ExtractRootMotionFromRange(const UAnimSequence* _animationSequence, float _startTrackPosition, float _endTrackPosition);
	UFUNCTION(BlueprintCallable, Category = "Joy|Animation") static FTransform ExtractRootTrackTransform(const UAnimSequence* _animationSequence, float _pos);

	UFUNCTION(BlueprintPure, Category = "Joy|Animation") static float GetPlayLength(const UAnimSequence* _animationSequence);

	UFUNCTION(BlueprintCallable, Category = "Joy|Animation") static void RefreshAnimationCache(UAnimSequence* _animationSequence);
};

