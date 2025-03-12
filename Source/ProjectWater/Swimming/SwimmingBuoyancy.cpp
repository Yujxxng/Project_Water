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

	//Set Collision Channel. Only Interact with Water
	cubeMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	cubeMeshComponent->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Block);

	//BuoyancyComponent = NewObject<UBuoyancyComponent>(this, TEXT("BuoyancyComponent"));
	BuoyancyComponent = CreateDefaultSubobject<UBuoyancyComponent>(TEXT("BuoyancyComponent"));
	//BuoyancyComponent->RegisterComponent();

	FSphericalPontoon Pontoon = FSphericalPontoon();
	Pontoon.CenterSocket = "MyPontoon";
	Pontoon.RelativeLocation = FVector(0.f, 0.f, 0.f);
	Pontoon.Radius = 100.f;
	Pontoon.bFXEnabled = true;

	BuoyancyComponent->BuoyancyData.bCenterPontoonsOnCOM = true;
	BuoyancyComponent->BuoyancyData.Pontoons.Add(Pontoon);
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

void ASwimmingBuoyancy::SetXYPosition()
{
	//Retains buoyancy by keeping the same Z value but follows the player by setting the X and Y values of the cube to the player's.

	if (IsValid(Player))
	{
		double X = Player->GetActorLocation().X;
		double Y = Player->GetActorLocation().Y;
		double Z = cubeMeshComponent->GetComponentLocation().Z;
		cubeMeshComponent->SetWorldLocation(FVector(X, Y, Z));
	}
}

