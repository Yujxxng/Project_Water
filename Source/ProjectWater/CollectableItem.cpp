// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectableItem.h"
#include "ItemDataTable.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ACollectableItem::ACollectableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//static ConstructorHelpers::FObjectFinder<UDataTable> ItemData(TEXT("/Game/CollectorBook/DT_ItemDataTable"));
	//if (ItemData.Succeeded())
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("DataTable Succeed!"));
	//	ItemDataTable = ItemData.Object;
	//}
	//else
	//	UE_LOG(LogTemp, Warning, TEXT("DataTable Fail"));
}

// Called when the game starts or when spawned
void ACollectableItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollectableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}