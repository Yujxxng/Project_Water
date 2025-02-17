// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemComponent.h"
#include "WaterGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UItemComponent::UItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UItemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	LoadItemPlayerHas();
}


// Called every frame
void UItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


bool UItemComponent::HasItem(FString ItemID)
{
	for (int i = 0; i < PlayerCollector.Num(); i++)
	{
		if (PlayerCollector[i].ID == ItemID)
			return PlayerCollector[i].IsCollected;
	}

	return false;
}

void UItemComponent::LoadItemPlayerHas()
{
	UWaterGameInstance* GameInstance = Cast<UWaterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		for (int i = 0; i < GameInstance->PlayerItem.Num(); i++)
		{
			PlayerCollector.Add(GameInstance->PlayerItem[i]);
			UE_LOG(LogTemp, Warning, TEXT("%s %d"), *GameInstance->PlayerItem[i].ID, GameInstance->PlayerItem[i].IsCollected);
		}
		//UE_LOG(LogTemp, Warning, TEXT("Item Load"));
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Item Load fail"));
}

