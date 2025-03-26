// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUtils.h"

MyUtils::MyUtils()
{
}

MyUtils::~MyUtils()
{
}

float MapRangeClamped(float value, float inRangeA, float inRangeB, float outRangeA, float outRangeB)
{
	if (inRangeA == inRangeB)
		return outRangeA;

	float Normalized = (value - inRangeA) / (outRangeB - outRangeA);
	float Result = Normalized * (outRangeB - outRangeA) + outRangeA;

	return FMath::Clamp(Result, outRangeA, outRangeB);
}
