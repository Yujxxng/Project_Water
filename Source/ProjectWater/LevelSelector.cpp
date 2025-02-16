// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelSelector.h"

ULevelSelector::ULevelSelector(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	LevelNames.Add(TEXT("Human_"), 3);
	LevelNames.Add(TEXT("Water_"), 1);
	LevelNames.Add(TEXT("Ice_"), 1);
	LevelNames.Add(TEXT("Vapor_"), 1);
}
