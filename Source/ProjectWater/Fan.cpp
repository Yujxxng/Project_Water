// Fill out your copyright notice in the Description page of Project Settings.


#include "Fan.h"

#include "Components/HierarchicalInstancedStaticMeshComponent.h"

// Sets default values
AFan::AFan()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HISM = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("HierarchicalInstancedStaticMesh"));
	HISM->SetupAttachment(RootComponent);

	
}

// Called when the game starts or when spawned
void AFan::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFan::OnConstruction(const FTransform& Transform)
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/StaticMeshes/SM_Cube_PivotZero"));
	HISM->SetStaticMesh(MeshAsset.Object);

	FBox boundingBox = SM->GetBoundingBox();
	
}

// Called every frame
void AFan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

