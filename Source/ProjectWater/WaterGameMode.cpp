// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterGameMode.h"
#include "WaterGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectWaterCharacter.h"
#include "CharacterState.h"
AWaterGameMode::AWaterGameMode()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> ItemData(TEXT("/Game/CollectorBook/DT_ItemDataTable"));
	if (ItemData.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("DataTable Succeed!"));
		ItemDataTable = ItemData.Object;
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("DataTable Fail"));
}
void AWaterGameMode::BeginPlay()
{
	Super::BeginPlay();
	if (ItemDataTable != nullptr)
	{
		auto DataList = ItemDataTable->GetRowNames();
		FName tempname = DataList[1];
		FItemTableRow* Itemtable = ItemDataTable->FindRow<FItemTableRow>(tempname, FString(""));
		UE_LOG(LogTemp, Log, TEXT("%s , %s"), *(Itemtable->ID), *(Itemtable->Name));
	}
}
void AWaterGameMode::UpdateStates()
{
	if (auto waterGameInstance = Cast<UWaterGameInstance>(GetGameInstance()))
	{
		ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
		if (PlayerCharacter)
		{
			AProjectWaterCharacter* MyCharacter = Cast<AProjectWaterCharacter>(PlayerCharacter);
			if (MyCharacter)
			{
				for (int i = 0; i < waterGameInstance->LockedStates.Num(); i++)
				{
					if(!MyCharacter->CharacterState->GetLockedState(i))
						waterGameInstance->LockedStates[i] = false;
				}
				
				waterGameInstance->HeartNum = MyCharacter->GetHearts();
			}

		}
		waterGameInstance->SaveGame();
		UE_LOG(LogTemp, Warning, TEXT("Save"));
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Fail to Get GameInstance in WaterGameMode"));
}
