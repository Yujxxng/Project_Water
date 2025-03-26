// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumSwimMode.generated.h"

UENUM(BlueprintType)
enum class SwimMode_Enum : uint8
{
	Treading,
	SurfaceSwimming,
	UnderwaterIdle,
	UnderwaterSwimming,
};

enum class WaterInteractionBones_Enum
{
	Hands,
	Feet,
};

UCLASS()
class PROJECTWATER_API AEnumSwimMode : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnumSwimMode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
