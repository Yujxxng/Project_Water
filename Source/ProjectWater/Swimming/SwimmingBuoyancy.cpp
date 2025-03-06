// Fill out your copyright notice in the Description page of Project Settings.

#include "SwimmingBuoyancy.h"

// Sets default values
ASwimmingBuoyancy::ASwimmingBuoyancy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a static mesh component
	cubeMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));

	// Load the Cube mesh
	cubeMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;

	// Set the component's mesh
	cubeMeshComponent->SetStaticMesh(cubeMesh);

	// Set as root component
	RootComponent = cubeMeshComponent;

	cubeMeshComponent->SetSimulatePhysics(true);
	cubeMeshComponent->SetVisibility(false);
	cubeMeshComponent->bVisibleInReflectionCaptures = false;
	cubeMeshComponent->bVisibleInRealTimeSkyCaptures = false;
	cubeMeshComponent->bVisibleInRayTracing = false;

}

// Called when the game starts or when spawned
void ASwimmingBuoyancy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASwimmingBuoyancy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

