// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemComponent.h"
#include "ItemDataTable.h"
#include "UObject/ConstructorHelpers.h"
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

	virtual void Init() override;

	void SaveGame();
	void LoadGame();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UDataTable* ItemDataTable;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TArray<FPlayerCollect> PlayerItem;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TArray<bool> LockedStates;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int HeartNum;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int MapLock;

	UFUNCTION(BlueprintCallable)
	void CreateSaveFile();

	UFUNCTION(BlueprintCallable)
	bool IsItemCollected(FString ItemID);
};
