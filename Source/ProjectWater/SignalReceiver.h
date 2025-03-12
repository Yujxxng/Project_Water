// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Interact.h"
#include "SignalReceiver.generated.h"

UCLASS()
class PROJECTWATER_API ASignalReceiver : public AActor, public IInteract
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASignalReceiver();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Interact() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
