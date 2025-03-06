// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SignalEmitter.generated.h"

class ASignalReceiver;

UCLASS()
class PROJECTWATER_API ASignalEmitter : public AActor
{
	GENERATED_BODY()
	
	TArray<ASignalReceiver*> Receivers;

public:	
	// Sets default values for this actor's properties
	ASignalEmitter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
