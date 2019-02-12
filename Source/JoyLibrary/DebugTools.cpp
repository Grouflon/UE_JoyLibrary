// Fill out your copyright notice in the Description page of Project Settings.

#include "DebugTools.h"

#include <Kismet/GameplayStatics.h>
#include <GameFramework/PlayerController.h>
#include <GameFramework/HUD.h>
#include <Engine/World.h>

#include <Assert.h>
#include <JoyHUD.h>

void ToggleShowDebugEnabled(UWorld* _world)
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(_world, 0);
	if (!playerController)
		return;

	playerController->ConsoleCommand(TEXT("ShowDebug"));

	
}

void SetShowDebugEnabled(UWorld* _world, bool _enabled)
{
	if (IsShowDebugEnabled(_world) == _enabled)
		return;

	ToggleShowDebugEnabled(_world);
}

bool IsShowDebugEnabled(UWorld* _world)
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(_world, 0);
	if (!playerController)
		return false;

	AHUD* hud = playerController->GetHUD();
	if (!hud)
		return false;

	return hud->bShowDebugInfo;
}

bool IsActorDebugTarget(AActor* _actor)
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(_actor, 0);
	if (!playerController)
		return false;

	AHUD* hud = playerController->GetHUD();
	if (!hud)
		return false;

	return hud->GetCurrentDebugTargetActor() == _actor;
}

void SetActorAsDebugTarget(AActor* _actor)
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(_actor, 0);
	if (!playerController)
		return;

	// NOTE(Remi|2018/06/26): Hacky hack because there is no way to set debug target directly.
	TArray<AActor*> debugList;
	playerController->GetHUD()->GetDebugActorList(debugList);
	AJoyHUD* joyHud = Cast<AJoyHUD>(playerController->GetHUD());
	JOY_ASSERT_MSG(joyHud, TEXT("HUD must inherit of AJoyHUD to be able to make this call."));

	if (debugList.Find(_actor) == INDEX_NONE)
	{
		joyHud->AddActorToDebugList(_actor);
		debugList.Empty();
		playerController->GetHUD()->GetDebugActorList(debugList);
	}

	int32 index = debugList.Find(_actor);
	JOY_ASSERT(index != INDEX_NONE);
	index = (index - 2 + debugList.Num()) % debugList.Num();
	playerController->GetHUD()->CurrentTargetIndex = index;
	playerController->GetHUD()->NextDebugTarget();
}
