// Fill out your copyright notice in the Description page of Project Settings.


#include "EmitterLever.h"
#include "Components/BoxComponent.h"

// Sets default values
AEmitterLever::AEmitterLever()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	Joint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Joint"));
	Arm = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arm"));
	Handle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Handle"));
	Wall1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall1"));
	Wall2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall2"));
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> circlePlate(TEXT("/Game/StaticMeshes/SM_CirclePlate.SM_CirclePlate"));
	Base->SetStaticMesh(circlePlate.Object);
	Wall1->SetStaticMesh(circlePlate.Object);
	Wall2->SetStaticMesh(circlePlate.Object);

	ConstructorHelpers::FObjectFinder<UStaticMesh> sphere(TEXT("/Game/StaticMeshes/SM_Sphere.SM_Sphere"));
	Joint->SetStaticMesh(sphere.Object);

	ConstructorHelpers::FObjectFinder<UStaticMesh> cylinder(TEXT("/Game/StaticMeshes/SM_Cylinder.SM_Cylinder"));
	Arm->SetStaticMesh(cylinder.Object);

	ConstructorHelpers::FObjectFinder<UStaticMesh> wideCapsule(TEXT("/Game/StaticMeshes/SM_WideCapsule.SM_WideCapsule"));
	Handle->SetStaticMesh(wideCapsule.Object);

	RootComponent = Base;
	Joint->SetupAttachment(Base);
	Arm->SetupAttachment(Joint);
	Handle->SetupAttachment(Arm);
	Wall1->SetupAttachment(Base);
	Wall2->SetupAttachment(Base);
	Box->SetupAttachment(Base);

	Base->SetRelativeScale3D(FVector(0.4, 0.4, 2.0));

	Joint->SetRelativeScale3D(FVector(0.4, 0.4, 0.08));
	Joint->SetRelativeLocationAndRotation(FVector(0, 0, 10), FRotator(-35, 0, 0).Quaternion());

	Arm->SetRelativeScale3D(FVector(0.3, 0.3, 4.8));

	Handle->SetRelativeScale3D(FVector(2, 2, 0.3));
	Handle->SetRelativeLocation(FVector(0, 0, 80));

	Wall1->SetRelativeScale3D(FVector(0.75, 0.75, 0.25));
	Wall1->SetRelativeLocationAndRotation(FVector(0, 12.5, 10), FRotator(0, 0, 90).Quaternion());

	Wall2->SetRelativeScale3D(FVector(0.75, 0.75, 0.25));
	Wall2->SetRelativeLocationAndRotation(FVector(0, -12.5, 10), FRotator(0, 180, 90).Quaternion());

	Box->SetRelativeScale3D(FVector(4.5, 4.5, 0.9));
	Box->SetRelativeLocation(FVector(0, 0, 30));
}

// Called when the game starts or when spawned
void AEmitterLever::BeginPlay()
{
	Super::BeginPlay();
}

void AEmitterLever::Interact()
{
	if (!bActive)
	{

	}
}

// Called every frame
void AEmitterLever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

