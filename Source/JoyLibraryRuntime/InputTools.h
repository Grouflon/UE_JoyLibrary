#pragma once

class UInputComponent;
class APlayerController;

JOYLIBRARYRUNTIME_API void RemoveActionBinding(UInputComponent* _inputComponent, const FName& _actionName);
JOYLIBRARYRUNTIME_API bool IsActionBindingPressed(APlayerController* _playerController, UInputComponent* _inputComponent, const FName& _actionName);
JOYLIBRARYRUNTIME_API void RemoveAxisBinding(UInputComponent* _inputComponent, const FName& _axisName);
