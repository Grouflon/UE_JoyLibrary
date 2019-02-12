// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>

JOYLIBRARY_API void ToggleShowDebugEnabled(UWorld* _world);
JOYLIBRARY_API void SetShowDebugEnabled(UWorld* _world, bool _enabled);
JOYLIBRARY_API bool IsShowDebugEnabled(UWorld* _world);
JOYLIBRARY_API bool IsActorDebugTarget(AActor* _actor);
JOYLIBRARY_API void SetActorAsDebugTarget(AActor* _actor);

