// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WaterGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "LevelSelector.generated.h"

USTRUCT(BlueprintType)
struct FLevelInformation
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	bool isClear;

	UPROPERTY(BlueprintReadOnly)
	FString Name;

	UPROPERTY(BlueprintReadOnly)
	FString DisplayName;

	UPROPERTY(BlueprintReadOnly)
	UTexture2D* Texture;
};

/**
 * 
 */
UCLASS()
class PROJECTWATER_API ULevelSelector : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FLevelInformation> LevelInfos;

public:
	ULevelSelector(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	bool IsLevelClear(int idx) { return LevelInfos[idx].isClear; }

	UFUNCTION(BlueprintCallable)
	void LoadLevelInfo();
};
