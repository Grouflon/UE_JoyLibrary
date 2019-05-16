// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>

JOYLIBRARYRUNTIME_API void ToggleShowDebugEnabled(UWorld* _world);
JOYLIBRARYRUNTIME_API void SetShowDebugEnabled(UWorld* _world, bool _enabled);
JOYLIBRARYRUNTIME_API bool IsShowDebugEnabled(UWorld* _world);
JOYLIBRARYRUNTIME_API bool IsActorDebugTarget(AActor* _actor);
JOYLIBRARYRUNTIME_API void SetActorAsDebugTarget(AActor* _actor);

