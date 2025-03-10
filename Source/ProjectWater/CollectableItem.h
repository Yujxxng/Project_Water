// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "CollectableItem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetItem, FString, itemID);

UCLASS()
class PROJECTWATER_API ACollectableItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollectableItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Detail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsCollected;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UTexture2D*> Textures;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* Mesh;

	UPROPERTY(BlueprintAssignable)
	FGetItem OnGetItem;
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UDataTable* ItemDataTable;*/
};
