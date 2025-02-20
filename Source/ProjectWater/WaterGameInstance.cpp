// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGameData.h"

UWaterGameInstance::UWaterGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> ItemData(TEXT("/Game/CollectorBook/DT_ItemTable"));
	if (ItemData.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("DataTable Succeed!"));
		ItemDataTable = ItemData.Object;
		for (auto& Row : ItemDataTable->GetRowMap())
		{
			FItemTableRow* Item = (FItemTableRow*)Row.Value;
			if (Item)
			{
				FPlayerCollect pc;
				pc.ID = Item->ID;
				pc.IsCollected = false;

				PlayerItem.Add(pc);
			}
		}
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("DataTable Fail"));

	LockedStates.Add(true);
	LockedStates.Add(true);
	LockedStates.Add(true);

	HeartNum = 3;

	TArray<FString> levelNames = {
		TEXT("Human_1"),
		TEXT("Human_2"),
		TEXT("Human_3"),
		TEXT("Water_1"),
		//TEXT("Water_2"),
		TEXT("Ice_1"),
		TEXT("Vapor_1"),
		TEXT("Vapor_2_1"),
		TEXT("Vapor_2_3"),
		TEXT("Vapor_2_4"),
	};

	LevelClear.Init(false, 9);
	for (int i = 0; i < 9; i++)
	{
		//LevelClear.Add(false);
		LevelName.Add(levelNames[i]);

	}

	
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
		dataToSave->MapLock = this->MapLock;
		dataToSave->PlayerItem = this->PlayerItem;
		dataToSave->LevelClear = this->LevelClear;

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
		this->MapLock = dataToLoad->MapLock;
		this->PlayerItem = dataToLoad->PlayerItem;
		this->LevelClear = dataToLoad->LevelClear;
	}
	else if (!UGameplayStatics::DoesSaveGameExist("Slot1", 0))
	{
		CreateSaveFile();
	}
}

bool UWaterGameInstance::IsItemCollected(FString ItemID)
{
	for (int i = 0; i < PlayerItem.Num(); i++)
	{
		if (PlayerItem[i].ID == ItemID)
			return PlayerItem[i].IsCollected;
	}

	return false;
}

int UWaterGameInstance::GetLevelIndex(FString _LevelName)
{
	for (int i = 0; i < NUM_LEVEL; i++)
	{
		if (_LevelName == LevelName[i])
			return i;
	}

	return -1;
}
