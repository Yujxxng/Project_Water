// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterGameMode.h"
#include "WaterGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectWaterCharacter.h"
#include "CharacterState.h"
#include "LevelSelector.h"

AWaterGameMode::AWaterGameMode()
{
	
}
void AWaterGameMode::BeginPlay()
{
	Super::BeginPlay();
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

void AWaterGameMode::UpdateCollector(FString itemID)
{
	if (auto waterGameInstance = Cast<UWaterGameInstance>(GetGameInstance()))
	{
		ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
		if (PlayerCharacter)
		{
			AProjectWaterCharacter* MyCharacter = Cast<AProjectWaterCharacter>(PlayerCharacter);
			if (MyCharacter)
			{
				for (int i = 0; i < waterGameInstance->PlayerItem.Num(); i++)
				{
					if(waterGameInstance->PlayerItem[i].ID == itemID)
						waterGameInstance->PlayerItem[i].IsCollected = true;
				}
			}

		}
		waterGameInstance->SaveGame();
		UE_LOG(LogTemp, Warning, TEXT("Save Collector"));
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Fail to Get GameInstance in WaterGameMode"));
}

void AWaterGameMode::UpdateMapLock(FString LevelName)
{
	if (auto waterGameInstance = Cast<UWaterGameInstance>(GetGameInstance()))
	{
		int LvIdx = waterGameInstance->GetLevelIndex(LevelName);
		if(LvIdx >= 0)
		{
			if (!waterGameInstance->LevelClear[LvIdx])
			{
				waterGameInstance->LevelClear[LvIdx] = true;
				waterGameInstance->MapLock++;
				waterGameInstance->SaveGame();
				UE_LOG(LogTemp, Warning, TEXT("Save Map Process"));
			}
		}
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Fail to Get GameInstance in WaterGameMode"));
}