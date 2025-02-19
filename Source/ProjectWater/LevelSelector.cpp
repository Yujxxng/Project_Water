// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelSelector.h"
#include "Engine/StreamableManager.h"
#include <Kismet/GameplayStatics.h>

ULevelSelector::ULevelSelector(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	UWaterGameInstance* GameInstance = Cast<UWaterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		for (int i = 0; i < NUM_LEVEL; i++)
		{
			LevelInfos.Add(FLevelInformation());
			LevelInfos[i].Name = GameInstance->LevelName[i];
			LevelInfos[i].isClear = GameInstance->LevelClear[i];

			ConstructorHelpers::FObjectFinder<UTexture2D> texture(TEXT("/Game/Textures/jumpdecal.jumpdecal"));
			LevelInfos[i].Texture = texture.Object;

		}
		ConstructorHelpers::FObjectFinder<UTexture2D> tmp(TEXT("/Game/Textures/heart.heart"));
		LevelInfos[0].Texture = tmp.Object;
	}
}

void ULevelSelector::LoadLevelInfo()
{
	UWaterGameInstance* GameInstance = Cast<UWaterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		for (int i = 0; i < NUM_LEVEL; i++)
		{
			LevelInfos.Add(FLevelInformation());
			LevelInfos[i].Name = GameInstance->LevelName[i];
			LevelInfos[i].isClear = GameInstance->LevelClear[i];

			ConstructorHelpers::FObjectFinder<UTexture2D> texture(TEXT("/Game/Textures/jumpdecal.jumpdecal"));
			LevelInfos[i].Texture = texture.Object;

		}
		ConstructorHelpers::FObjectFinder<UTexture2D> tmp(TEXT("/Game/Textures/heart.heart"));
		LevelInfos[0].Texture = tmp.Object;
	}
}
