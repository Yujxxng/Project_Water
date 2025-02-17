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
				UE_LOG(LogTemp, Warning, TEXT("%s"), *Item->Name);
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
}

void UWaterGameInstance::Init()
{
	Super::Init();

	//Load the game upon initializing the WaterGameInstance
	LoadGame();

	//if (ItemDataTable != nullptr)
	//{
	//	auto DataList = ItemDataTable->GetRowNames();
	//	FName tempname = DataList[1];
	//	FItemTableRow* Itemtable = ItemDataTable->FindRow<FItemTableRow>(tempname, FString(""));
	//	UE_LOG(LogTemp, Log, TEXT("%s , %s"), *(Itemtable->ID), *(Itemtable->Name));
	//}
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
