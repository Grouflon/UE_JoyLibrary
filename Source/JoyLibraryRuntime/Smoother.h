// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Smoother.generated.h"

template<typename T>
JOYLIBRARYRUNTIME_API FORCEINLINE void DampSpring(T& _v0, const T& _target, T& _vel, float _time90, float _dt)
{
	if (_time90 <= SMALL_NUMBER)
	{
		_v0 = _target;
		_vel = T(0.0f);
		return;
	}

	const float c0 = _dt * 3.75f / _time90;

	if (c0 >= 1.0f)
	{
		// here, constant is too small, spring too stiff.  so go the whole way to prevent oscillation.
		_v0 = _target;
		_vel = T(0.0f);
		return;
	}

	const T delta = _target - _v0;
	const T force = delta - 2.0f * _vel;

	_v0 += _vel * c0;

	_vel += force * c0;
}

JOYLIBRARYRUNTIME_API void DampSpring(FQuat& _v0, const FQuat& _target, FQuat& _vel, float _time90, float _dt);

USTRUCT(BlueprintType)
struct JOYLIBRARYRUNTIME_API FSmootherFloat
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite) float TimeTo90Percent = 0.f;
	UPROPERTY(BlueprintReadWrite) float Value = 0.f;
	UPROPERTY(BlueprintReadWrite) float Velocity = 0.f;
};

USTRUCT(BlueprintType)
struct JOYLIBRARYRUNTIME_API FSmootherVector
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite) float TimeTo90Percent = 0.f;
	UPROPERTY(BlueprintReadWrite) FVector Value = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite) FVector Velocity = FVector::ZeroVector;
};

USTRUCT(BlueprintType)
struct JOYLIBRARYRUNTIME_API FSmootherRotation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite) float TimeTo90Percent = 0.f;
	UPROPERTY(BlueprintReadWrite) FQuat Value = FQuat::Identity;
	UPROPERTY(BlueprintReadWrite) FQuat Velocity = FQuat::Identity;
};

/**
 * 
 */
UCLASS()
class JOYLIBRARYRUNTIME_API USmoother : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Joy|Smoother") static void ResetSmootherFloat(UPARAM(ref) FSmootherFloat& _smoother, float _value = 0.f);
	UFUNCTION(BlueprintCallable, Category = "Joy|Smoother") static void SetTimeTo90PercentFloat(UPARAM(ref) FSmootherFloat& _smoother, float _timeTo90Percent);
	UFUNCTION(BlueprintCallable, Category = "Joy|Smoother") static float SmoothFloat(UPARAM(ref) FSmootherFloat& _smoother, float _target, float _deltaTime);
	
	UFUNCTION(BlueprintCallable, Category = "Joy|Smoother") static void ResetSmootherVector(UPARAM(ref) FSmootherVector& _smoother, FVector _value = FVector::ZeroVector);
	UFUNCTION(BlueprintCallable, Category = "Joy|Smoother") static void SetTimeTo90PercentVector(UPARAM(ref) FSmootherVector& _smoother, float _timeTo90Percent);
	UFUNCTION(BlueprintCallable, Category = "Joy|Smoother") static FVector SmoothVector(UPARAM(ref) FSmootherVector& _smoother, FVector _target, float _deltaTime);

	UFUNCTION(BlueprintCallable, Category = "Joy|Smoother") static void ResetSmootherRotation(UPARAM(ref) FSmootherRotation& _smoother, FRotator _value = FRotator::ZeroRotator);
	UFUNCTION(BlueprintCallable, Category = "Joy|Smoother") static void SetTimeTo90PercentRotation(UPARAM(ref) FSmootherRotation& _smoother, float _timeTo90Percent);
	UFUNCTION(BlueprintCallable, Category = "Joy|Smoother") static FRotator SmoothRotation(UPARAM(ref) FSmootherRotation& _smoother, FRotator _target, float _deltaTime);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joy|Smoother") static FRotator GetRotator(const FSmootherRotation& _smoother);
	static void ResetSmootherRotation(UPARAM(ref) FSmootherRotation& _smoother, FQuat _value = FQuat::Identity);
	static FQuat SmoothRotation(UPARAM(ref) FSmootherRotation& _smoother, FQuat _target, float _deltaTime);
};
