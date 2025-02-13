// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "WorldRoad.generated.h"

class UInstancedStaticMeshComponent;
class USplineComponent;
class UCurveFloat;

UCLASS()
class PROJECTWATER_API AWorldRoad : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UInstancedStaticMeshComponent* InstancedStaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USplineComponent* Spline;

	UStaticMesh* SplineMesh;

	float Spacing;
	float Multiplier;

	FVector GetLocationAlongSpline(int idx);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Timeline, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTimelineComponent> Timeline;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Timeline, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCurveFloat> Curve;

	FOnTimelineFloat MoveCallback;
	UFUNCTION()
	void MoveTimeLine(float value);

public:	
	// Sets default values for this actor's properties
	AWorldRoad();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Offset;

	
};
