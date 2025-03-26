// Fill out your copyright notice in the Description page of Project Settings.


#include "SwimActorEnterCheck.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASwimActorEnterCheck::ASwimActorEnterCheck()
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

	//Set Collision Channel. Only Interact with Water
	cubeMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	cubeMeshComponent->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Block);

	BuoyancyComponent = CreateDefaultSubobject<UBuoyancyComponent>(TEXT("BuoyancyComponent"));


	FSphericalPontoon Pontoon = FSphericalPontoon();
	Pontoon.CenterSocket = "MyPontoon";
	Pontoon.RelativeLocation = FVector(0.f, 0.f, 0.f);
	Pontoon.Radius = 100.f;
	Pontoon.bFXEnabled = true;

	BuoyancyComponent->BuoyancyData.bCenterPontoonsOnCOM = true;
	BuoyancyComponent->BuoyancyData.Pontoons.Add(Pontoon);

	//Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	
}

// Called when the game starts or when spawned
void ASwimActorEnterCheck::BeginPlay()
{
	Super::BeginPlay();
	
	if (BuoyancyComponent)
	{
		FScriptDelegate EnteredWaterDelegate;
		EnteredWaterDelegate.BindUFunction(this, FName("StartSwimming"));

		BuoyancyComponent->OnEnteredWaterDelegate.Add(EnteredWaterDelegate);
	}
}

// Called every frame
void ASwimActorEnterCheck::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASwimActorEnterCheck::StartSwimming(const FSphericalPontoon& Pontoon)
{
}

