// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "WaterGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTWATER_API UWaterGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UWaterGameInstance();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TArray<bool> LockedStates;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int HeartNum;
};
