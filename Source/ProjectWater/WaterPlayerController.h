// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WaterPlayerController.generated.h"

class AProjectWaterCharacter;
class ASignalEmitter;

/**
 * 
 */
UCLASS()
class PROJECTWATER_API AWaterPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	AProjectWaterCharacter* PlayerCharacter;

	const float BlendTime = 0.75f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void SetAttentionCamera(const FVector& CameraLocation, const FVector& AttentionLocation, const ASignalEmitter* Emitter, bool bActive);
	void ResetAttentionCamera();
};
