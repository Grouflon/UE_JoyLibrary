#include "AnimationTools.h"

#include <Log.h>
#include <Assert.h>
#include <Animation/AnimInstance.h>

JOYLIBRARY_API float GetAnimationStateRelevantAnimTimeRemaining(UAnimInstance* _animInstance, FName _stateMachineName, FName _stateName)
{
	// FIND ANIM TOTAL TIME
	int32 machineIndex = _animInstance->GetStateMachineIndex(_stateMachineName);
	if (machineIndex == INDEX_NONE)
	{
		LOGF_WARNING("Unknown State Machine \"%s\".", *_stateMachineName.ToString());
		return 0.f;
	}
	const FBakedAnimationStateMachine* machinePtr = _animInstance->GetStateMachineInstanceDesc(_stateMachineName);
	JOY_ASSERT(machinePtr);
	int32 stateIndex = machinePtr->FindStateIndex(_stateName);
	if (stateIndex == INDEX_NONE)
	{
		LOGF_WARNING("Unknown State \"%s\" in StateMachine \"%s\".", *_stateMachineName.ToString(), *_stateName.ToString());
		return 0.f;
	}

	return _animInstance->GetRelevantAnimTimeRemaining(machineIndex, stateIndex);
}
