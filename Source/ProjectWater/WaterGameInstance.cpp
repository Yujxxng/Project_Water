// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGameData.h"

UWaterGameInstance::UWaterGameInstance()
{
	LockedStates.Add(true);
	LockedStates.Add(true);
	LockedStates.Add(true);

	HeartNum = 3;
}

void UWaterGameInstance::Init()
{
	Super::Init();

	//Load the game upon initializing the WaterGameInstance
	LoadGame();
}

void UWaterGameInstance::CreateSaveFile()
{
	//Create a SaveGame Object and save to the default slot.
	USaveGameData* dataToSave = Cast<USaveGameData>(UGameplayStatics::CreateSaveGameObject(USaveGameData::StaticClass()));
	UGameplayStatics::SaveGameToSlot(dataToSave, "Slot1", 0);
}

void UWaterGameInstance::SaveGame()
{
	//Initialize data to save.
	USaveGameData* dataToSave = Cast<USaveGameData>(UGameplayStatics::LoadGameFromSlot("Slot1", 0));

	//If there is a valid SaveGame object to use for saving.
	if (dataToSave != nullptr)
	{
		dataToSave->LockedStates = this->LockedStates;
		dataToSave->HeartNum = this->HeartNum;
		UGameplayStatics::SaveGameToSlot(dataToSave, "Slot1", 0);
	}
	else if (!UGameplayStatics::DoesSaveGameExist("Slot1", 0))
	{
		CreateSaveFile();
	}
}

void UWaterGameInstance::LoadGame()
{
	//Retrieve data to load.
	USaveGameData* dataToLoad = Cast<USaveGameData>(UGameplayStatics::LoadGameFromSlot("Slot1", 0));

	//If there is valid data to load.
	if (dataToLoad != nullptr)
	{
		this->LockedStates = dataToLoad->LockedStates;
		this->HeartNum = dataToLoad->HeartNum;
	}
	else if (!UGameplayStatics::DoesSaveGameExist("Slot1", 0))
	{
		CreateSaveFile();
	}
}
