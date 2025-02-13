// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldRoad.h"
#include "ProjectWaterCharacter.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Math/Transform.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Curves/CurveFloat.h"

// Sets default values
AWorldRoad::AWorldRoad()
	: Offset(30.f), Spacing(0.f), Multiplier(0.1f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InstancedStaticMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedStaticMesh"));
	
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetupAttachment(InstancedStaticMesh);

	SetRootComponent(InstancedStaticMesh);

	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("/Game/StaticMeshes/SM_Road.SM_Road"));
	if (mesh.Succeeded())
	{
		SplineMesh = mesh.Object;
		InstancedStaticMesh->SetStaticMesh(SplineMesh);
	}
	
	Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Movement"));
	MoveCallback.BindUFunction(this, FName("MoveTimeLine"));
	ConstructorHelpers::FObjectFinder<UCurveFloat> curve(TEXT("/Game/Actor/World/MoveCurve.MoveCurve"));
	if (curve.Succeeded())
	{
		Curve = curve.Object;
	}
}

FVector AWorldRoad::GetLocationAlongSpline(int idx)
{
	return Spline->GetLocationAtDistanceAlongSpline(Spacing * idx, ESplineCoordinateSpace::Local);
}

void AWorldRoad::MoveTimeLine(float value)
{
	static AProjectWaterCharacter* player = Cast<AProjectWaterCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	float length = Spline->GetSplineLength();
	static FVector location;
	location = GetActorLocation() + Spline->GetLocationAtDistanceAlongSpline(length * value, ESplineCoordinateSpace::Local);

	if (player)
	{
		UE_LOG(LogTemp, Warning, TEXT("location %f %f"), location.X, location.Y);
		player->SetActorLocation(location);
	}
}

// Called when the game starts or when spawned
void AWorldRoad::BeginPlay()
{
	Super::BeginPlay();
	
	Timeline->AddInterpFloat(Curve, MoveCallback);
	Timeline->SetTimelineLength(1.f);
	Timeline->SetLooping(false);
}

void AWorldRoad::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (IsValid(InstancedStaticMesh))
	{
		InstancedStaticMesh->ClearInstances();
	}

	FBox box = SplineMesh->GetBoundingBox();
	float splineMeshSizeX = (box.Max - box.Min).X;
	Spacing = Offset + splineMeshSizeX;

	int splineMeshNum = Spline->GetSplineLength() / Spacing;

	FVector curLocation = GetLocationAlongSpline(0);
	for (int i = 1; i <= splineMeshNum; i++)
	{
		FVector nextLocation = GetLocationAlongSpline(i);

		FTransform transform;
		transform.SetLocation(curLocation);
		transform.SetRotation(FQuat(UKismetMathLibrary::MakeRotFromX(nextLocation - curLocation)));
		transform.SetScale3D(FVector(1.f, 1.f, 1.f));
		InstancedStaticMesh->AddInstance(transform);

		curLocation = nextLocation;
	}
}

// Called every frame
void AWorldRoad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

