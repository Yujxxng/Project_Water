// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterPlayerController.h"
#include "ProjectWaterCharacter.h"
#include "PuzzleSystem/SignalEmitter.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "TimerManager.h"

void AWaterPlayerController::BeginPlay()
{
	PlayerCharacter = Cast<AProjectWaterCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void AWaterPlayerController::SetAttentionCamera(const FVector& CameraLocation, const FVector& AttentionLocation, const ASignalEmitter* Emitter, bool bActive)
{
	PlayerCharacter->SetIgnoreInput(true);

	FRotator rotator = (CameraLocation - AttentionLocation).Rotation();
	FTransform transform(rotator, CameraLocation);
	PlayerCharacter->AttentionCamera->SetActorTransform(transform);

	SetViewTargetWithBlend(PlayerCharacter->AttentionCamera, BlendTime);

	FTimerHandle setAttentionHandle;
	if (bActive)
	{
		GetWorld()->GetTimerManager().SetTimer(
			setAttentionHandle,
			Emitter,
			&ASignalEmitter::ActiveReceivers,
			BlendTime,
			false,
			0.f
		);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(
			setAttentionHandle,
			Emitter,
			&ASignalEmitter::InactiveReceivers,
			BlendTime,
			false,
			0.f
		);
	}
}

void AWaterPlayerController::ResetAttentionCamera()
{
	SetViewTargetWithBlend(PlayerCharacter, BlendTime);

	FTimerHandle handle;
	FTimerDelegate delegate = FTimerDelegate::CreateUObject(PlayerCharacter, &AProjectWaterCharacter::SetIgnoreInput, false);
	GetWorld()->GetTimerManager().SetTimer(
		handle,
		delegate,
		BlendTime,
		false
	);
}
