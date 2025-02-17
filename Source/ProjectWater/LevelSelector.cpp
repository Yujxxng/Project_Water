// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelSelector.h"

ULevelSelector::ULevelSelector(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TArray<FString> levelNames = {
		TEXT("Human_1"),
		TEXT("Human_2"),
		TEXT("Human_3"),
		TEXT("Water_1"),
		TEXT("Ice_1"),
		TEXT("Vapor_1"),
	};

	for (int i = 0; i < NUM_LEVEL; i++)
	{
		LevelInfos.Add(FLevelInformation());

		LevelInfos[i].Name = levelNames[i];

		ConstructorHelpers::FObjectFinder<UTexture2D> texture(TEXT("/Game/Textures/jumpdecal.jumpdecal"));
		LevelInfos[i].Texture = texture.Object;
	}

	ConstructorHelpers::FObjectFinder<UTexture2D> tmp(TEXT("/Game/Textures/heart.heart"));
	LevelInfos[0].Texture = tmp.Object;
}
