// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldRoad.h"
#include "Components/InstancedStaticMeshComponent.h"

// Sets default values
AWorldRoad::AWorldRoad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//InstancedStaticMesh->RegisterComponent();
}

// Called when the game starts or when spawned
void AWorldRoad::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWorldRoad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

