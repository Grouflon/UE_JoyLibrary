// Fill out your copyright notice in the Description page of Project Settings.

#include "Smoother.h"
#include <Assert.h>

void DampSpring(FQuat& _v0, const FQuat& _target, FQuat& _vel, float _time90, float _dt)
{
	if (_time90 <= SMALL_NUMBER)
	{
		_v0 = _target;
		_vel = FQuat::Identity;
		return;
	}

	const float c0 = _dt * 3.75f / _time90;

	if (c0 >= 1.0f)
	{
		// here, constant is too small, spring too stiff.  so go the whole way to prevent oscillation.
		_v0 = _target;
		_vel = FQuat::Identity;
		return;
	}

	/*const T delta = _target - _v0;
	const T force = delta - 2.0f * _vel;

	_v0 += _vel * c0;

	_vel += force * c0;*/

	_v0.EnforceShortestArcWith(_target);

	FQuat target = _target; // NOTE(Remi|2018/12/21): Copying the reference to a local variable to avoid weird memory alignment crash in DebugGame.
	const FQuat delta = _v0.Inverse() * target;
	
	const FQuat force = (_vel * _vel).Inverse() * delta;

	_v0 *= FQuat::Slerp(FQuat::Identity, _vel, c0);
	_vel *= FQuat::Slerp(FQuat::Identity, force, c0);
}

void USmoother::ResetSmootherFloat(FSmootherFloat& _smoother, float _value /*= 0.f*/)
{
	_smoother.Value = _value;
	_smoother.Velocity = 0.f;
}

void USmoother::SetTimeTo90PercentFloat(UPARAM(ref) FSmootherFloat& _smoother, float _timeTo90Percent)
{
	_smoother.TimeTo90Percent = _timeTo90Percent;
}

float USmoother::SmoothFloat(FSmootherFloat& _smoother, float _target, float _deltaTime)
{
	JOY_ASSERT(!IsNaN(_target) && IsFinite(_target));
	DampSpring(_smoother.Value, _target, _smoother.Velocity, _smoother.TimeTo90Percent, _deltaTime);
	JOY_ASSERT(!IsNaN(_smoother.Value) && IsFinite(_smoother.Value));
	return _smoother.Value;
}

void USmoother::ResetSmootherVector(FSmootherVector& _smoother, FVector _value /*= FVector::ZeroVector*/)
{
	_smoother.Value = _value;
	_smoother.Velocity = FVector::ZeroVector;
}

void USmoother::SetTimeTo90PercentVector(UPARAM(ref) FSmootherVector& _smoother, float _timeTo90Percent)
{
	_smoother.TimeTo90Percent = _timeTo90Percent;
}

FVector USmoother::SmoothVector(FSmootherVector& _smoother, FVector _target, float _deltaTime)
{
	JOY_ASSERT(!IsNaN(_target) && IsFinite(_target));
	DampSpring(_smoother.Value, _target, _smoother.Velocity, _smoother.TimeTo90Percent, _deltaTime);
	JOY_ASSERT(!IsNaN(_smoother.Value) && IsFinite(_smoother.Value));
	return _smoother.Value;
}

FRotator USmoother::GetRotator(const FSmootherRotation& _smoother)
{
	return _smoother.Value.Rotator();
}

void USmoother::ResetSmootherRotation(FSmootherRotation& _smoother, FQuat _value /*= FQuat::Identity*/)
{
	_smoother.Value = _value;
	_smoother.Velocity = FQuat::Identity;
}

FQuat USmoother::SmoothRotation(FSmootherRotation& _smoother, FQuat _target, float _deltaTime)
{
	JOY_ASSERT(!IsNaN(_target) && IsFinite(_target));
	DampSpring(_smoother.Value, _target, _smoother.Velocity, _smoother.TimeTo90Percent, _deltaTime);
	JOY_ASSERT(!IsNaN(_smoother.Value) && IsFinite(_smoother.Value));
	return _smoother.Value;
}

void USmoother::ResetSmootherRotation(FSmootherRotation& _smoother, FRotator _value /*= FRotator::ZeroRotator*/)
{
	ResetSmootherRotation(_smoother, _value.Quaternion());
}

FRotator USmoother::SmoothRotation(FSmootherRotation& _smoother, FRotator _target, float _deltaTime)
{
	return SmoothRotation(_smoother, _target.Quaternion(), _deltaTime).Rotator();
}

void USmoother::SetTimeTo90PercentRotation(FSmootherRotation& _smoother, float _timeTo90Percent)
{
	_smoother.TimeTo90Percent = _timeTo90Percent;
}
