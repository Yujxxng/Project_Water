// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class PROJECTWATER_API MyUtils
{
public:
	MyUtils();
	~MyUtils();

};

float MapRangeClamped(float value, float inRangeA, float inRangeB, float outRangeA, float outRangeB);
