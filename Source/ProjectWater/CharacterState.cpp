// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterState.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectWaterCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

// Sets default values for this component's properties
UCharacterState::UCharacterState()
	: Owner(nullptr), MovementComponent(nullptr)
	, State(EState::STATE_Human)
	, MaxEnergy(100.f), Energy(MaxEnergy)
	, EnergyRecovery(0.3f), EnergyUsage(0.2f)
	, EnergyCheckInterval(200), EnergyTimerStart()
	, bExhausted(false), isCharacterStateChanged(false)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetState(EState::STATE_Human);

	LockedState[0] = true;
	LockedState[1] = true;
	LockedState[2] = true;
}


// Called when the game starts
void UCharacterState::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<AProjectWaterCharacter>(GetOwner<AProjectWaterCharacter>());
	MovementComponent = Owner->GetCharacterMovement();
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
		if (bExhausted || State == EState::STATE_Human)
		{
			Energy += EnergyRecovery;
		}
		else
		{
			Energy -= EnergyUsage;
		}

		//UE_LOG(LogTemp, Log, TEXT("Energy %f"), Energy);
		adder -= EnergyCheckInterval;
	}

	if (!bExhausted && Energy <= 0.f)
	{
		Energy = 0.f;
		SetState(EState::STATE_Human);
		MovementComponent->MaxWalkSpeed *= 0.1f;
		//Owner->DamageHearts();
		UGameplayStatics::ApplyDamage(Owner, 1.f, Owner->GetInstigatorController(), nullptr, NULL);

		bExhausted = true;
		EnergyRecovery = 0.6f;
	}
	
	if (bExhausted && Energy >= MaxEnergy)
	{
		bExhausted = false;
		EnergyRecovery = 0.3f;

		Energy = MaxEnergy;
		State = EState::STATE_None;
		SetState(EState::STATE_Human);
	}
}

void UCharacterState::SetState(EState newState)
{
	if (bExhausted || State == newState ||
		(newState != EState::STATE_Human && LockedState[int(newState) - 2]))
	{
		UE_LOG(LogTemp, Log, TEXT("SetState fail %d"), newState);
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
		MovementComponent->AirControl = 0.4f;
		MovementComponent->GravityScale = 1.75f;
		MovementComponent->Mass = 100.f;
		break;

	case EState::STATE_Water:
		UE_LOG(LogTemp, Log, TEXT("EState::STATE_Water"));
		MovementComponent->GroundFriction = 6.f;
		MovementComponent->MaxWalkSpeed = 420.f;
		MovementComponent->MaxAcceleration = 500.f;
		MovementComponent->BrakingDecelerationWalking = 1500.f;
		MovementComponent->BrakingDecelerationFalling = 1500.f;
		MovementComponent->AirControl = 0.3f;
		MovementComponent->GravityScale = 1.75f;
		MovementComponent->Mass = 100.f;
		break;

	case EState::STATE_Vapor:
		UE_LOG(LogTemp, Log, TEXT("EState::STATE_Vapor"));
		MovementComponent->GroundFriction = 5.f;
		MovementComponent->MaxWalkSpeed = 450.f;
		MovementComponent->MaxAcceleration = 2000.f;
		MovementComponent->BrakingDecelerationWalking = 1000.f;
		MovementComponent->BrakingDecelerationFalling = 200.f;
		MovementComponent->AirControl = 1.f;
		MovementComponent->GravityScale = 0.5f;
		MovementComponent->Mass = 1.f;
		break;

	case EState::STATE_Ice:
		UE_LOG(LogTemp, Log, TEXT("EState::STATE_Ice"));
		MovementComponent->GroundFriction = 2.f;
		MovementComponent->MaxWalkSpeed = 1800.f;
		MovementComponent->MaxAcceleration = 500.f;
		MovementComponent->BrakingDecelerationWalking = 100.f;
		MovementComponent->BrakingDecelerationFalling = 1500.f;
		MovementComponent->AirControl = 0.3f;
		MovementComponent->GravityScale = 1.75f;
		MovementComponent->Mass = 100.f;
		break;
	}

	State = newState;
	EnergyTimerStart = std::chrono::system_clock::now();
	isCharacterStateChanged = true;

	switch (State)
	{
		case EState::STATE_Water:
		case EState::STATE_Vapor:
			Owner->GetCapsuleComponent()->SetCollisionProfileName("FluidState");
			break;

		//case EState::STATE_Ice:
		//	Owner->GetCapsuleComponent()->SetCollisionProfileName("IceState");
		//	break;

		default:
			Owner->GetCapsuleComponent()->SetCollisionProfileName("SolidState");
	}
}

void UCharacterState::UnlockState(EState unlock)
{
	LockedState[int(unlock) - 2] = false;
}

bool UCharacterState::GetLockedState(EState unlock)
{
	return LockedState[int(unlock) - 2];
}
