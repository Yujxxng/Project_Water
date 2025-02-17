// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "ItemDataTable.generated.h"

USTRUCT(BlueprintType)
struct FItemTableRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString Detail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool IsCollected;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TArray<UTexture2D*> Textures;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UStaticMesh* Mesh;
};

UCLASS()
class PROJECTWATER_API AItemDataTable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemDataTable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
