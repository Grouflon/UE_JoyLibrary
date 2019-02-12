#pragma once

class UInputComponent;
class APlayerController;

JOYLIBRARY_API void RemoveActionBinding(UInputComponent* _inputComponent, const FName& _actionName);
JOYLIBRARY_API bool IsActionBindingPressed(APlayerController* _playerController, UInputComponent* _inputComponent, const FName& _actionName);
JOYLIBRARY_API void HackyRemoveAxisBindingThatMightCorruptMemory(UInputComponent* _inputComponent, const FName& _axisName);
