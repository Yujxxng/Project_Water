// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldRoad.h"
#include "ProjectWaterCharacter.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Math/Transform.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Curves/CurveFloat.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AWorldRoad::AWorldRoad()
	: Player(nullptr)
	, Spacing(0.f), Multiplier(0.1f), Offset(30.f)
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
	//static const FVector forward(0.f, 1.f, 0.f);
	//static const FVector right(1.f, 0.f, 0.f);

	float length = Spline->GetSplineLength();
	FVector location = Spline->GetLocationAtDistanceAlongSpline(length * value, ESplineCoordinateSpace::World);
	//UE_LOG(LogTemp, Log, TEXT("value %f | location %f %f"), value, location.X, location.Y);
	if (IsValid(Player))
	{
		FVector playerLocation = Player->GetActorLocation();
		FVector direction = location - playerLocation;
		direction.Z = 0.f;

		location.Z = playerLocation.Z;
		Player->SetActorLocation(location);
		
		const FRotator rotation = UKismetMathLibrary::FindLookAtRotation(playerLocation, location);
		Player->SetActorRotation(FQuat(rotation));
		
		FVector forward = Player->GetActorForwardVector();
		FVector right = Player->GetActorRightVector();

		//UE_LOG(LogTemp, Log, TEXT("direction %f %f || forward %f %f"), direction.X, direction.Y, forward.X, forward.Y);
		//UE_LOG(LogTemp, Log, TEXT("dot product %f"), FVector::DotProduct(forward, direction));
		//Player->GetCharacterMovement()->MaxWalkSpeed *= length;
		//Player->GetMesh()->SetPhysicsLinearVelocity(FVector(length, 0.f, 0.f));
		//Player->AddMovementInput(forward, FVector::DotProduct(forward, direction));
		//Player->AddMovementInput(right, FVector::DotProduct(right, direction));
	}
}

void AWorldRoad::Construct()
{
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

// Called when the game starts or when spawned
void AWorldRoad::BeginPlay()
{
	Super::BeginPlay();

	Construct();
	
	Player = Cast<AProjectWaterCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	Timeline->AddInterpFloat(Curve, MoveCallback);
	Timeline->SetTimelineLength(1.f);
	Timeline->SetLooping(false);
}

void AWorldRoad::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	Construct();
}

// Called every frame
void AWorldRoad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

