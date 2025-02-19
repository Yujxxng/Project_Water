// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemComponent.h"
#include "ItemDataTable.h"
#include "LevelSelector.h"
#include "GameFramework/SaveGame.h"
#include "SaveGameData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTWATER_API USaveGameData : public USaveGame
{
	GENERATED_BODY()
	
public:
	USaveGameData();

	UPROPERTY(VisibleAnywhere)
	TArray<FPlayerCollect> PlayerItem;

	UPROPERTY(VisibleAnywhere)
	TArray<bool> LockedStates;

	UPROPERTY(VisibleAnywhere)
	int HeartNum;

	UPROPERTY(VisibleAnywhere)
	int MapLock;

	UPROPERTY(VisibleAnywhere)
	TArray<bool> LevelClear;
};
