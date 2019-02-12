// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class AManager;
class UCurveFloat;
class AHUD;
class UCanvas;
class FDebugDisplayInfo;

#include "CoreMinimal.h"
#include "Kismet/BlueprintPlatformLibrary.h"

#include <CoordinatesOrigin.h>

#include "JoyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class JOYLIBRARY_API UJoyGameInstance : public UPlatformGameInstance
{
	GENERATED_BODY()

public:
	virtual void OnStart() override;
	virtual void Shutdown() override;

	UFUNCTION(BlueprintCallable, BlueprintPure) AManager* GetManager(TSubclassOf<AManager> _managerClass) const;
	template <typename T> T* GetManager() const;
	
	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = 8)) void DrawDebugCurve(UCurveFloat* _curve, ECoordinatesOrigin _coordinatesOrigin, FVector2D _position, FVector2D _size, float _curveXMin, float _curveXMax, float _curveValue, const FString& _curveName, FColor _curveColor = FColor::Red, FColor _valueColor = FColor::Yellow, int _samplesCount = 64);

	void _InstantiateManagers();
	TSubclassOf<AManager> _FindDeepestChildClass(TSubclassOf<AManager> _managerClass) const;
	void _OnHudShowDebugInfo(AHUD* _HUD, UCanvas* _canvas, const FDebugDisplayInfo& _displayInfo, float& YL, float& YPos);

	struct FDebugCurveData
	{
		UCurveFloat* Curve;
		FBox2D Location;
		float CurveXMin;
		float CurveXMax;
		float CurveValue;
		FString CurveName;
		FColor CurveColor;
		FColor ValueColor;
		uint32 SamplesCount;
	};
	TArray<FDebugCurveData> m_debugCurveData;

	UPROPERTY() mutable TArray<AManager*> m_managers;
};

template <typename T>
T* UJoyGameInstance::GetManager() const
{
	return Cast<T>(GetManager(T::StaticClass()));
}
