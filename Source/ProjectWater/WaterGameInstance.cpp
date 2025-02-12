// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterGameInstance.h"
#include "Kismet/GameplayStatics.h"

UWaterGameInstance::UWaterGameInstance()
{
	LockedStates.Add(true);
	LockedStates.Add(true);
	LockedStates.Add(true);

	HeartNum = 3;
}
