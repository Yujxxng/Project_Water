// Fill out your copyright notice in the Description page of Project Settings.


#include "SignalReceiver.h"

// Sets default values
ASignalReceiver::ASignalReceiver()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASignalReceiver::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASignalReceiver::ReceiveOnSignal()
{
	Activate();
}

void ASignalReceiver::ReceiveOffSignal()
{
	Inactivate();
}

// Called every frame
void ASignalReceiver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

