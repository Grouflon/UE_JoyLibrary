#include "InputTools.h"

#include <Components/InputComponent.h>
#include <GameFramework/PlayerInput.h>
#include <GameFramework/PlayerController.h>

#include <Assert.h>

void RemoveActionBinding(UInputComponent* _inputComponent, const FName& _actionName)
{
	JOY_ASSERT(_inputComponent);

	bool found = false;
	for (uint16 i = 0; i < _inputComponent->GetNumActionBindings();)
	{
		FInputActionBinding binding = _inputComponent->GetActionBinding(i);
		if (binding.GetActionName() == _actionName)
		{
			found = true;
			_inputComponent->RemoveActionBinding(i);
		}
		else
		{
			++i;
		}
	}

	JOY_ASSERT_MSGF(found, TEXT("Failed to remove action binding \"%s\"."), *_actionName.ToString());
}

bool IsActionBindingPressed(APlayerController* _playerController, UInputComponent* _inputComponent, const FName& _actionName)
{
	JOY_ASSERT(_playerController);
	JOY_ASSERT(_inputComponent);

	for (uint16 i = 0; i < _inputComponent->GetNumActionBindings(); ++i)
	{
		FInputActionBinding binding = _inputComponent->GetActionBinding(i);
		if (binding.GetActionName() == _actionName)
		{
			for (const FInputActionKeyMapping& keyMapping : _playerController->PlayerInput->GetKeysForAction(binding.GetActionName()))
			{
				if (_playerController->PlayerInput->IsPressed(keyMapping.Key))
				{
					return true;
				}
			}
		}
	}
	return false;
}

void HackyRemoveAxisBindingThatMightCorruptMemory(UInputComponent* _inputComponent, const FName& _axisName)
{
	JOY_ASSERT(_inputComponent);

#if WITH_EDITOR
	uint64 arrayOffset = 0x0000000000000120;
#else
	uint64 arrayOffset = 0x0000000000000118;
#endif

	// NOTE(Remi|2018/04/02): Just measured the offset between AxisBindings member and the base pointer to _inputComponent, but it may vary between platforms and build types
	TArray<struct FInputAxisBinding>& axis = *(TArray<struct FInputAxisBinding>*)((char*)_inputComponent + arrayOffset);

	bool found = false;
	for (uint16 i = 0; i < axis.Num();)
	{
		FInputAxisBinding binding = axis[i];
		if (binding.AxisName == _axisName)
		{
			found = true;
			axis.RemoveAt(i);
		}
		else
		{
			++i;
		}
	}

	JOY_ASSERT_MSGF(found, TEXT("Failed to remove axis binding \"%s\"."), *_axisName.ToString());
}
