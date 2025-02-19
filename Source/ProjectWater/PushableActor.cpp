// Fill out your copyright notice in the Description page of Project Settings.


#include "PushableActor.h"
#include "ProjectWaterCharacter.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APushableActor::APushableActor()
	: PlayerCharacter(nullptr), InitialInputValue()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APushableActor::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerCharacter = Cast<AProjectWaterCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called every frame
void APushableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APushableActor::SetInitialInputValue()
{
	if (PlayerCharacter)
	{
		InitialInputValue = PlayerCharacter->GetPlayerInputActionValue().Get<FVector>();
	}
}

bool APushableActor::CheckPlayerInputForPushing()
{
	if (PlayerCharacter)
	{
		FVector curInputValue = PlayerCharacter->GetPlayerInputActionValue().Get<FVector>();
		if (InitialInputValue == curInputValue)
		{
			return true;
		}
	}
	return false;
}

