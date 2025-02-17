// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGameData.h"

USaveGameData::USaveGameData()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> ItemData(TEXT("/Game/CollectorBook/DT_ItemTable"));
	class UDataTable* ItemDataTable;
	if (ItemData.Succeeded())
	{
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
	MapLock = 0;
}
