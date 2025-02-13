// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDataTable.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/GameModeBase.h"
#include "WaterGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTWATER_API AWaterGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AWaterGameMode();
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void UpdateStates();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UDataTable* ItemDataTable;
};
