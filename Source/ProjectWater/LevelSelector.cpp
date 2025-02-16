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

	for (const FString& levelName : levelNames)
	{
		ConstructorHelpers::FObjectFinder<UTexture2D> texture(TEXT("/Game/Textures/jumpdecal.jumpdecal"));
		LevelTextures.Add(levelName, texture.Object);
	}

	ConstructorHelpers::FObjectFinder<UTexture2D> tmp(TEXT("/Game/Textures/heart.heart"));
	LevelTextures.Add(TEXT("Human_1"), tmp.Object);
}
