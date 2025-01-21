// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterState.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UCharacterState::UCharacterState()
	: MovementComponent(nullptr), State(EState::STATE_Human)
	, Energy(100.f), EnergyRecovery(0.3f), EnergyUsage(0.2f)
	, EnergyCheckInterval(200), EnergyTimerStart()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetState(EState::STATE_Human);
}


// Called when the game starts
void UCharacterState::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* owner = Cast<ACharacter>(GetOwner());
	MovementComponent = owner->GetCharacterMovement();
}


// Called every frame
void UCharacterState::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (State == EState::STATE_Human && Energy >= 100.f)
	{
		return;
	}

	static std::chrono::duration<long, std::milli> adder{ 0 };

	std::chrono::system_clock::time_point cur = std::chrono::system_clock::now();
	adder += std::chrono::duration_cast<std::chrono::milliseconds>(cur - EnergyTimerStart);
	if (adder >= EnergyCheckInterval)
	{
		EnergyTimerStart = cur;
	}

	while (adder >= EnergyCheckInterval)
	{
		switch (State)
		{
		case EState::STATE_Human:
			Energy += EnergyRecovery;
			break;

		default:
			Energy -= EnergyUsage;
		}
		UE_LOG(LogTemp, Log, TEXT("Energy %f"), Energy);

		adder -= EnergyCheckInterval;
	}
}

void UCharacterState::SetState(EState newState)
{
	if (State == newState)
	{
		return;
	}

	switch (newState)
	{
	case EState::STATE_Human:
		UE_LOG(LogTemp, Log, TEXT("EState::STATE_Human"));
		MovementComponent->GroundFriction = 8.f;
		MovementComponent->MaxWalkSpeed = 600.f;
		MovementComponent->MaxAcceleration = 2000.f;
		MovementComponent->BrakingDecelerationWalking = 2000.f;
		MovementComponent->BrakingDecelerationFalling = 1500.f;
		MovementComponent->AirControl = 0.2f;
		MovementComponent->GravityScale = 1.75f;
		MovementComponent->JumpZVelocity = 420.f;
		MovementComponent->Mass = 100.f;
		break;

	case EState::STATE_Water:
		UE_LOG(LogTemp, Log, TEXT("EState::STATE_Water"));
		MovementComponent->GroundFriction = 6.f;
		MovementComponent->MaxWalkSpeed = 420.f;
		MovementComponent->MaxAcceleration = 500.f;
		MovementComponent->BrakingDecelerationWalking = 1500.f;
		MovementComponent->BrakingDecelerationFalling = 1500.f;
		MovementComponent->AirControl = 0.2f;
		MovementComponent->GravityScale = 1.75f;
		MovementComponent->JumpZVelocity = 300.f;
		MovementComponent->Mass = 125.f;
		break;

	case EState::STATE_Vapor:
		UE_LOG(LogTemp, Log, TEXT("EState::STATE_Vapor"));
		MovementComponent->GroundFriction = 5.f;
		MovementComponent->MaxWalkSpeed = 450.f;
		MovementComponent->MaxAcceleration = 2000.f;
		MovementComponent->BrakingDecelerationWalking = 1000.f;
		MovementComponent->BrakingDecelerationFalling = 1000.f;
		MovementComponent->AirControl = 0.7f;
		MovementComponent->GravityScale = 1.f;
		MovementComponent->JumpZVelocity = 600.f;
		MovementComponent->Mass = 1.f;
		break;

	case EState::STATE_Ice:
		UE_LOG(LogTemp, Log, TEXT("EState::STATE_Ice"));
		MovementComponent->GroundFriction = 2.f;
		MovementComponent->MaxWalkSpeed = 1800.f;
		MovementComponent->MaxAcceleration = 500.f;
		MovementComponent->BrakingDecelerationWalking = 100.f;
		MovementComponent->BrakingDecelerationFalling = 1500.f;
		MovementComponent->AirControl = 0.2f;
		MovementComponent->GravityScale = 1.75f;
		MovementComponent->JumpZVelocity = 300.f;
		MovementComponent->Mass = 125.f;
		break;
	}

	State = newState;
	EnergyTimerStart = std::chrono::system_clock::now();
}

