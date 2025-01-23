// Fill out your copyright notice in the Description page of Project Settings.


#include "Fan.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AFan::AFan()
	: CollisionComponent(nullptr), FanForce(1000.f)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;

	CollisionComponent->SetCollisionProfileName(TEXT("Trigger"));
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AFan::OnOverlapBegin);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AFan::OnOverlapEnd);
}

void AFan::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* character = Cast<ACharacter>(OtherActor);
	if (character)
	{
		character->GetCharacterMovement()->AddForce(FVector(0.f, 0.f, FanForce));
	}
}

void AFan::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

// Called when the game starts or when spawned
void AFan::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

