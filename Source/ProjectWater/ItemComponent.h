// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "ItemComponent.generated.h"


USTRUCT(BlueprintType)
struct FPlayerCollect : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsCollected;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTWATER_API UItemComponent : public UActorComponent
{
	GENERATED_BODY()
	TArray<FPlayerCollect> PlayerCollector;
public:	
	// Sets default values for this component's properties
	UItemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintGetter)
	TArray<FPlayerCollect> GetCollector() const { return PlayerCollector; }
	//UFUNCTION(BlueprintSetter)
	//void SetItemCollected(FString ItemID);

	UFUNCTION(BlueprintCallable)
	bool HasItem(FString ItemID);

	UFUNCTION(BlueprintCallable)
	void LoadItemPlayerHas();
		
};
