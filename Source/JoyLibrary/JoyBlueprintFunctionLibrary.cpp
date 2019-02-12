// Fill out your copyright notice in the Description page of Project Settings.

#include "JoyBlueprintFunctionLibrary.h"

#include <CoreGlobals.h>
#include <Kismet/GameplayStatics.h>
#include <Components/WidgetComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include <Components/ShapeComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Animation/AnimSingleNodeInstance.h>
#include <Navigation/PathFollowingComponent.h>
#include <NavigationPath.h>
#include <AIController.h>

#include <Assert.h>
#include <DebugTools.h>
#include <SplineTools.h>
#include <Math.h>
#include <JoyGameInstance.h>

int UJoyBlueprintFunctionLibrary::GetCurrentFrameNumber()
{
	return GFrameNumber;
}

void UJoyBlueprintFunctionLibrary::SetWidgetComponentWidgetClass(UWidgetComponent* _component, TSubclassOf<UUserWidget> _class)
{
	_component->SetWidgetClass(_class);
}

bool UJoyBlueprintFunctionLibrary::IsWithEditorBuild()
{
#if WITH_EDITOR
	return true;
#else
	return false;
#endif
}

bool UJoyBlueprintFunctionLibrary::IsAssertEnabled()
{
#if JOY_ASSERT_ENABLED
	return true;
#else
	return false;
#endif
}

void UJoyBlueprintFunctionLibrary::Assert(bool _condition, FString _message)
{
	JOY_ASSERT_MSGF(_condition, TEXT("%s"), *_message);
}

int UJoyBlueprintFunctionLibrary::HashString(FString _string)
{
	const uint32 DJB2_INIT = 5381;

	if (_string.Len() == 0)
		return 0;

	uint32 hash = DJB2_INIT;

	for (int i = 0; i < _string.Len(); ++i)
	{
		TCHAR c = _string.GetCharArray()[i];
		hash = ((hash << 5) + hash) + c;
	}

	return hash;
}

void UJoyBlueprintFunctionLibrary::SetShapeNavigationRelevancy(UShapeComponent* _shapeComponent, bool _relevancy)
{
	_shapeComponent->SetCanEverAffectNavigation(_relevancy);
}

bool UJoyBlueprintFunctionLibrary::GetShapeNavigationRelevancy(UShapeComponent* _shapeComponent)
{
	return _shapeComponent->CanEverAffectNavigation();
}

void UJoyBlueprintFunctionLibrary::GetPathComponentCurrentPath(const UPathFollowingComponent* _pathFollowingComponent, UNavigationPath* _outPath)
{
	const FNavPathSharedPtr Path = _pathFollowingComponent->GetPath();
	_outPath->SetPath(Path);
}

float UJoyBlueprintFunctionLibrary::GetPathComponentRemainingLength(const UPathFollowingComponent* _pathFollowingComponent)
{
	if (!_pathFollowingComponent->GetPath().IsValid() || _pathFollowingComponent->GetPath()->GetPathPoints().Num() == 0)
		return 0.f;

	AAIController* AIController = Cast<AAIController>(_pathFollowingComponent->GetOwner());
	return _pathFollowingComponent->GetPath()->GetLengthFromPosition(AIController->GetPawn()->GetActorLocation(), _pathFollowingComponent->GetNextPathIndex());
}

bool UJoyBlueprintFunctionLibrary::HasActorBegunPlay(const AActor* _actor)
{
	return _actor->HasActorBegunPlay();
}

float UJoyBlueprintFunctionLibrary::GetAngleBetweenRotators(const FRotator& _A, const FRotator& _B)
{
	return FMath::RadiansToDegrees(_A.Quaternion().AngularDistance(_B.Quaternion()));
}

float UJoyBlueprintFunctionLibrary::GetSignedAngleBetweenRotators(const FRotator& _A, const FRotator& _B)
{
	return FMath::RadiansToDegrees(SignedAngleBetween(_A.Quaternion().GetForwardVector(), _B.Quaternion().GetForwardVector(), _A.Quaternion().GetUpVector()));
}

FVector UJoyBlueprintFunctionLibrary::GetXY(const FVector& _vector)
{
	return ::GetXY(_vector);
}

FVector2D UJoyBlueprintFunctionLibrary::GetXY2D(const FVector& _vector)
{
	return ::GetXY2D(_vector);
}

FVector UJoyBlueprintFunctionLibrary::GetXZ(const FVector& _vector)
{
	return ::GetXZ(_vector);
}

FVector2D UJoyBlueprintFunctionLibrary::GetXZ2D(const FVector& _vector)
{
	return ::GetXZ2D(_vector);
}

FVector UJoyBlueprintFunctionLibrary::GetYZ(const FVector& _vector)
{
	return ::GetYZ(_vector);
}

FVector2D UJoyBlueprintFunctionLibrary::GetYZ2D(const FVector& _vector)
{
	return ::GetYZ2D(_vector);
}

float UJoyBlueprintFunctionLibrary::ComputeSquaredDistanceToPoint(const FBox2D& _box, const FVector2D& _point)
{
	return _box.ComputeSquaredDistanceToPoint(_point);
}

float UJoyBlueprintFunctionLibrary::AngleBetween(const FVector& _A, const FVector& _B)
{
	return FMath::RadiansToDegrees(::AngleBetween(_A, _B));
}

float UJoyBlueprintFunctionLibrary::SignedAngleBetween(const FVector& _A, const FVector& _B, FVector _UpVector/* = FVector::UpVector*/)
{
	return FMath::RadiansToDegrees(::SignedAngleBetween(_A, _B, _UpVector));
}

float UJoyBlueprintFunctionLibrary::FindDistanceAlongSplineClosestToWorldLocation(const USplineComponent* _spline, const FVector& _worldLocation)
{
	return ::FindDistanceAlongSplineClosestToWorldLocation(_spline, _worldLocation);
}

FBox UJoyBlueprintFunctionLibrary::GetActorVisualBounds(const AActor* _actor)
{
	TArray<UActorComponent*> staticMeshComponents = _actor->GetComponentsByClass(UStaticMeshComponent::StaticClass());
	if (staticMeshComponents.Num() == 0)
		return FBox(_actor->GetActorLocation(), _actor->GetActorLocation());
	
	FBox result = Cast<UStaticMeshComponent>(staticMeshComponents[0])->Bounds.GetBox();
	for (int32 i = 1; i < staticMeshComponents.Num(); ++i)
	{
		result = CombineBoxes(result, Cast<UStaticMeshComponent>(staticMeshComponents[i])->Bounds.GetBox());
	}
	return result;
}

FBox UJoyBlueprintFunctionLibrary::CombineBoxes(const FBox& _A, const FBox& _B)
{
	return FBox
	(
		FVector
		(
			FMath::Min(_A.Min.X, _B.Min.X),
			FMath::Min(_A.Min.Y, _B.Min.Y),
			FMath::Min(_A.Min.Z, _B.Min.Z)
		),
		FVector
		(
			FMath::Max(_A.Max.X, _B.Max.X),
			FMath::Max(_A.Max.Y, _B.Max.Y),
			FMath::Max(_A.Max.Z, _B.Max.Z)
		)
	);
}

FVector UJoyBlueprintFunctionLibrary::GetBoxCenter(const FBox& _box)
{
	return _box.GetCenter();
}

FVector UJoyBlueprintFunctionLibrary::GetBoxExtent(const FBox& _box)
{
	return _box.GetExtent();
}

float UJoyBlueprintFunctionLibrary::GetBoxRadius(const FBox& _box)
{
	return _box.GetExtent().Size();
}

UAnimationAsset* UJoyBlueprintFunctionLibrary::GetAnimation(USkeletalMeshComponent* _skeletalMeshComponent)
{
	JOY_ASSERT(_skeletalMeshComponent);
	return _skeletalMeshComponent->GetSingleNodeInstance() ? _skeletalMeshComponent->GetSingleNodeInstance()->GetAnimationAsset() : nullptr;
}

FTransform UJoyBlueprintFunctionLibrary::ConsumeRootMotion(USkeletalMeshComponent* _skeletalMeshComponent)
{
	JOY_ASSERT(_skeletalMeshComponent);
	return _skeletalMeshComponent->ConsumeRootMotion().GetRootMotionTransform();
}

void UJoyBlueprintFunctionLibrary::DrawDebugCurve(UObject* _worldContextObject, UCurveFloat* _curve, ECoordinatesOrigin _coordinatesOrigin, FVector2D _position, FVector2D _size, float _curveXMin, float _curveXMax, float _curveValue, const FString& _curveName, FColor _curveColor , FColor _valueColor , int _samplesCount)
{
	if (!_curve)
		return;

	AActor* actor = Cast<AActor>(_worldContextObject);
	if (actor && !IsActorDebugTarget(actor))
		return;

	UJoyGameInstance* joyGameInstance = Cast<UJoyGameInstance>(UGameplayStatics::GetGameInstance(_worldContextObject));
	if (joyGameInstance)
	{
		joyGameInstance->DrawDebugCurve(_curve, _coordinatesOrigin, _position, _size, _curveXMin, _curveXMax, _curveValue, _curveName, _curveColor, _valueColor, _samplesCount);
	}
}
