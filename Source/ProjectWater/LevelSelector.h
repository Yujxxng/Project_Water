// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "LevelSelector.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTWATER_API ULevelSelector : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<FString, UTexture2D*> LevelTextures;

public:
	ULevelSelector(const FObjectInitializer& ObjectInitializer);
	
};
