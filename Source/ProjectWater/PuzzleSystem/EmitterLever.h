// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "SignalEmitter.h"
#include "Interact.h"
#include "Components/TimelineComponent.h"
#include "EmitterLever.generated.h"

class UBoxComponent;
class UCurveFloat;

UCLASS()
class PROJECTWATER_API AEmitterLever : public ASignalEmitter, public IInteract
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Base;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Joint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Arm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Handle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Wall1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Wall2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> Box;

	const float Angle = 35.f;

	TObjectPtr<UTimelineComponent> Timeline;
	TObjectPtr<UCurveFloat> Curve;
	FOnTimelineFloat MoveCallback;

public:	
	// Sets default values for this actor's properties
	AEmitterLever();

private:
	UFUNCTION()
	void MoveTimeLine(float value);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Interact() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
