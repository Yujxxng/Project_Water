// Fill out your copyright notice in the Description page of Project Settings.

#include "SignalEmitter.h"
#include "SignalReceiver.h"
#include "../WaterPlayerController.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASignalEmitter::ASignalEmitter()
	: PlayerMovement(nullptr), PlayerController(nullptr)
	, bActive(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASignalEmitter::BeginPlay()
{
	Super::BeginPlay();

	auto world = GetWorld();
	PlayerMovement = UGameplayStatics::GetPlayerCharacter(world, 0)->GetCharacterMovement();
	PlayerController = Cast<AWaterPlayerController>(UGameplayStatics::GetPlayerController(world, 0));

	InitializeAttention();
}

void ASignalEmitter::InitializeAttention()
{
	if (!(bOnAttention || bOffAttention))
		return;

	FVector location = GetActorLocation();
	FRotator rotator = GetActorRotation();
	CameraLocation = location + rotator.RotateVector(CameraLocation);

	if (!AttentionLocation.IsNearlyZero())
	{
		AttentionLocation = location + rotator.RotateVector(AttentionLocation);
	}
	else
	{
		for (auto receiver : Receivers)
		{
			AttentionLocation += receiver->GetActorLocation();
		}
		AttentionLocation /= Receivers.Num();
	}

	//UE_LOG(LogTemp, Log, TEXT("CameraLocation %f %f %f"), CameraLocation.X, CameraLocation.Y, CameraLocation.Z);
	//UE_LOG(LogTemp, Log, TEXT("AttentionLocation %f %f %f"), AttentionLocation.X, AttentionLocation.Y, AttentionLocation.Z);
}

void ASignalEmitter::PlaySound() const
{
	// 사운드 변수 protected로 만들어서 하위 클래스에서 수정할 수 있게 만들어두기
	// 사운드 출력
}

void ASignalEmitter::EmitOnSignal()
{
	bActive = true;

	if (bOnAttention)
	{
		if (!bRepeatAttention)
		{
			bOnAttention = false;
		}

		PlayerMovement->Velocity = FVector::Zero();
		PlayerController->SetAttentionCamera(CameraLocation, AttentionLocation, this, true);
	}
	else
	{
		ActiveReceivers();
	}
}

void ASignalEmitter::EmitOffSignal()
{
	if (!bActive)
		return;

	bActive = false;

	if (bOffAttention)
	{
		if (!bRepeatAttention)
		{
			bOffAttention = false;
		}

		PlayerMovement->Velocity = FVector::Zero();
		// Call set second camera
	}
	else
	{
		InactiveReceivers();
	}
}

// Called every frame
void ASignalEmitter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASignalEmitter::ActiveReceivers() const
{
	if (bActive)
	{
		PlaySound();

		for (auto receiver : Receivers)
		{
			receiver->ReceiveOnSignal();
		}

		if (bOnAttention)
		{
			//FTimerHandle activeReceiverHandle;
			//GetWorld()->GetTimerManager().SetTimer(
			//	activeReceiverHandle, this, ASignalEmitter::
			//);
			// Call reset second camera
		}
	}
	else
	{
		// Call reset second camera
	}
}

void ASignalEmitter::InactiveReceivers() const
{
	for (auto receiver : Receivers)
	{
		receiver->ReceiveOffSignal();
	}

	if (bOffAttention)
	{
		//FTimerHandle activeReceiverHandle;
		//GetWorld()->GetTimerManager().SetTimer(
		//	activeReceiverHandle, this, ASignalEmitter::
		//);
		// Call reset second camera
	}
}