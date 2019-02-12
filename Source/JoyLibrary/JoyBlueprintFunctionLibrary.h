// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class UPathFollowingComponent;
class UNavigationPath;
class UCameraComponent;
class USkeletalMeshComponent;
class UAnimationAsset;
class USplineComponent;

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include <Engine/EngineTypes.h>

#include <CoordinatesOrigin.h>

#include "JoyBlueprintFunctionLibrary.generated.h"


/**
 * 
 */
UCLASS()
class JOYLIBRARY_API UJoyBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joy|Debug")	static int GetCurrentFrameNumber();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joy|Debug") static bool IsWithEditorBuild();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joy|Debug") static bool IsAssertEnabled();
	UFUNCTION(BlueprintCallable, Category = "Joy|Debug", meta = (WorldContext = "_worldContextObject", AdvancedDisplay = 9)) static void DrawDebugCurve(UObject* _worldContextObject, UCurveFloat* _curve, ECoordinatesOrigin _coordinatesOrigin, FVector2D _position, FVector2D _size, float _curveXMin, float _curveXMax, float _curveValue, const FString& _curveName, FColor _curveColor = FColor::Red, FColor _valueColor = FColor::Yellow, int _samplesCount = 64);
	UFUNCTION(BlueprintCallable, Category = "Joy|Debug") static void Assert(bool _condition, FString _message);

	UFUNCTION(BlueprintCallable, Category = "Joy") static void SetWidgetComponentWidgetClass(UWidgetComponent* _component, TSubclassOf<UUserWidget> _class);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joy") static int HashString(FString _string);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joy|Navigation") static bool GetShapeNavigationRelevancy(UShapeComponent* _shapeComponent);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joy|Navigation") static float GetPathComponentRemainingLength(const UPathFollowingComponent* _pathFollowingComponent);
	UFUNCTION(BlueprintCallable, Category = "Joy|Navigation") static void SetShapeNavigationRelevancy(UShapeComponent* _shapeComponent, bool _relevancy);
	UFUNCTION(BlueprintCallable, Category = "Joy|Navigation") static void GetPathComponentCurrentPath(const UPathFollowingComponent* _pathFollowingComponent, UNavigationPath* _outPath);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joy|Actor") static bool HasActorBegunPlay(const AActor* _actor);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joy|Math|Rotator", meta = (ToolTip = "Return the shortest angle between two rotators (in degrees)")) static float GetAngleBetweenRotators(const FRotator& _A, const FRotator& _B);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joy|Math|Rotator", meta = (ToolTip = "Return the shortest signed angle between two rotators (in degrees)")) static float GetSignedAngleBetweenRotators(const FRotator& _A, const FRotator& _B);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joy|Math") static FVector GetXY(const FVector& _vector);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joy|Math") static FVector2D GetXY2D(const FVector& _vector);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joy|Math") static FVector GetXZ(const FVector& _vector);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joy|Math") static FVector2D GetXZ2D(const FVector& _vector);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joy|Math") static FVector GetYZ(const FVector& _vector);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joy|Math") static FVector2D GetYZ2D(const FVector& _vector);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joy|Math") static float ComputeSquaredDistanceToPoint(const FBox2D& _box, const FVector2D& _point);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joy|Math") static float AngleBetween(const FVector& _A, const FVector& _B);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joy|Math", meta = (AdvancedDisplay = 2)) static float SignedAngleBetween(const FVector& _A, const FVector& _B, FVector _UpVector = FVector::UpVector);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joy|Spline") static float FindDistanceAlongSplineClosestToWorldLocation(const USplineComponent* _spline, const FVector& _worldLocation);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joy") static FBox GetActorVisualBounds(const AActor* _actor);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joy") static FBox CombineBoxes(const FBox& _A, const FBox& _B);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joy") static FVector GetBoxCenter(const FBox& _box);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joy") static FVector GetBoxExtent(const FBox& _box);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joy") static float GetBoxRadius(const FBox& _box);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Joy|Animation") static UAnimationAsset* GetAnimation(USkeletalMeshComponent* _skeletalMeshComponent);
	UFUNCTION(BlueprintCallable, Category = "Joy|Animation") static FTransform ConsumeRootMotion(USkeletalMeshComponent* _skeletalMeshComponent);

};
