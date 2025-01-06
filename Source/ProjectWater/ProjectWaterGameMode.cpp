// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectWaterGameMode.h"
#include "ProjectWaterCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectWaterGameMode::AProjectWaterGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
