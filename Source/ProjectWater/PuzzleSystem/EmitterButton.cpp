// Fill out your copyright notice in the Description page of Project Settings.


#include "EmitterButton.h"
#include "Components/BoxComponent.h"

// Sets default values
AEmitterButton::AEmitterButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	Plate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plate"));
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> circlePlate(TEXT("/Game/StaticMeshes/SM_CirclePlate.SM_CirclePlate"));
	Base->SetStaticMesh(circlePlate.Object);
	Plate->SetStaticMesh(circlePlate.Object);

	RootComponent = Root;
	Base->SetupAttachment(Root);
	Plate->SetupAttachment(Root);
	Box->SetupAttachment(Root);

	Base->SetRelativeScale3D(FVector(0.5, 0.5, 1.0));

	Plate->SetRelativeScale3D(FVector(0.4, 0.4, 2.0));
	Plate->SetRelativeLocation(FVector(0, 0, 5));

	Box->SetRelativeLocation(FVector(0, 0, 20));
	Box->OnComponentBeginOverlap.AddDynamic(this, &AEmitterButton::OnBoxBeginOverlap);
	//Box->OnComponentEndOverlap.AddDynamic(this, &AEmitterButton::OnBoxEndOverlap);	////////// 여기 다시 찾아보기

	Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Movement"));
	ConstructorHelpers::FObjectFinder<UCurveFloat> curve(TEXT("/Game/Curves/FloatCurve_-1to1.FloatCurve_-1to1"));
	if (curve.Succeeded())
	{
		Curve = curve.Object;
	}
}

void AEmitterButton::MoveTimeLine(float value)
{
	UE_LOG(LogTemp, Log, TEXT("MoveTimeLine %f"), value);
	Plate->SetRelativeLocation(FVector(0, 0, 5.f * value));
}

// Called when the game starts or when spawned
void AEmitterButton::BeginPlay()
{
	Super::BeginPlay();
	
	MoveCallback.BindUFunction(this, FName("MoveTimeLine"));
	Timeline->AddInterpFloat(Curve, MoveCallback);
	Timeline->SetPlayRate(2.f);
}

// Called every frame
void AEmitterButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEmitterButton::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("OnBoxBeginOverlap"));
	
	if (!bActive)
	{
		EmitOnSignal();
		Timeline->ReverseFromEnd();
	}
}

void AEmitterButton::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("OnBoxEndOverlap"));

	if (bActive)
	{
		EmitOffSignal();
		Timeline->PlayFromStart();
	}
}

