// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PushableActor.generated.h"

class AProjectWaterCharacter;

UCLASS()
class PROJECTWATER_API APushableActor : public AActor
{
	GENERATED_BODY()

	AProjectWaterCharacter* PlayerCharacter;
	FVector InitialInputValue;
	
public:	
	// Sets default values for this actor's properties
	APushableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetInitialInputValue();

	UFUNCTION(BlueprintCallable)
	bool CheckPlayerInputForPushing();
};
