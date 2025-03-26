// Fill out your copyright notice in the Description page of Project Settings.


#include "EmitterLever.h"
#include "Components/BoxComponent.h"
#include "Curves/CurveFloat.h"

// Sets default values
AEmitterLever::AEmitterLever()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

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

	RootComponent = Root;
	Base->SetupAttachment(Root);
	Joint->SetupAttachment(Base);
	Arm->SetupAttachment(Joint);
	Handle->SetupAttachment(Arm);
	Wall1->SetupAttachment(Root);
	Wall2->SetupAttachment(Root);
	Box->SetupAttachment(Root);

	Base->SetRelativeScale3D(FVector(0.4, 0.4, 2.0));

	Joint->SetRelativeScale3D(FVector(0.4, 0.4, 0.08));
	Joint->SetRelativeLocationAndRotation(FVector(0, 0, 10), FRotator(Angle, 0, 0).Quaternion());

	Arm->SetRelativeScale3D(FVector(0.3, 0.3, 4.8));

	Handle->SetRelativeScale3D(FVector(2, 2, 0.3));
	Handle->SetRelativeLocation(FVector(0, 0, 80));

	Wall1->SetRelativeScale3D(FVector(0.3, 0.3, 0.5));
	Wall1->SetRelativeLocationAndRotation(FVector(0, 5, 20), FRotator(0, 0, 90).Quaternion());

	Wall2->SetRelativeScale3D(FVector(0.3, 0.3, 0.5));
	Wall2->SetRelativeLocationAndRotation(FVector(0, -5, 20), FRotator(0, 180, 90).Quaternion());

	Box->SetRelativeScale3D(FVector(1.8, 1.8, 1.8));
	Box->SetRelativeLocation(FVector(0, 0, 60));

	Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Movement"));
	ConstructorHelpers::FObjectFinder<UCurveFloat> curve(TEXT("/Game/Curves/FloatCurve_-1to1.FloatCurve_-1to1"));
	if (curve.Succeeded())
	{
		Curve = curve.Object;
	}
}

void AEmitterLever::MoveTimeLine(float value)
{
	Joint->SetRelativeRotation(FRotator(Angle * value, 0, 0));
}

// Called when the game starts or when spawned
void AEmitterLever::BeginPlay()
{
	ASignalEmitter::BeginPlay();
	
	MoveCallback.BindUFunction(this, FName("MoveTimeLine"));
	Timeline->AddInterpFloat(Curve, MoveCallback);
	Timeline->SetPlayRate(2.f);

	if (bActive)
	{
		Joint->SetRelativeRotation(FRotator(Angle, 0, 0));
	}
	else
	{
		Joint->SetRelativeRotation(FRotator(-Angle, 0, 0));
	}
}

void AEmitterLever::Interact()
{
	if (!bActive)
	{
		EmitOnSignal();
		Timeline->Play();
	}
	else
	{
		EmitOffSignal();
		Timeline->Reverse();
	}
}

// Called every frame
void AEmitterLever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

