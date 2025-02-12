// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGameData.h"

USaveGameData::USaveGameData()
{
	LockedStates.Add(true);
	LockedStates.Add(true);
	LockedStates.Add(true);

	HeartNum = 3;
}
