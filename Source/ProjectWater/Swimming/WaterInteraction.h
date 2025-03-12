// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WaterInteraction.generated.h"

enum class SwimMode_Enum;
enum class WaterInteractionBones_Enum;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWaterInteraction : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTWATER_API IWaterInteraction
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void StartSwimming() = 0;
	virtual void EndSwimming() = 0;
	//virtual void AddWaterInteraction(WaterInteractionBones_Enum Bone, float SizeMultiplier);
	//virtual void GetSwimVariables(float Swimming, SwimMode_Enum SwimMode, float SwimSpeed, float SprintSwimSpecs) = 0;
};
