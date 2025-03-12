// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "SignalReceiver.generated.h"

UCLASS(Abstract)
class PROJECTWATER_API ASignalReceiver : public AActor
{	
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ASignalReceiver();

private:

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Activate() PURE_VIRTUAL(ASignalReceiver::Activate, );
	virtual void Inactivate() PURE_VIRTUAL(ASignalReceiver::Inactivate, );

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ReceiveOnSignal();
	void ReceiveOffSignal();

};
