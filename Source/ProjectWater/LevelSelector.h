// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelSelector.generated.h"

#define NUM_LEVEL 6

USTRUCT(BlueprintType)
struct FLevelInformation
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString Name;

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
	
};
