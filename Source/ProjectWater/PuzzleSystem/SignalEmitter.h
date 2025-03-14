// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SignalEmitter.generated.h"

class ASignalReceiver;
class UCharacterMovementComponent;
class AWaterPlayerController;

UCLASS(Abstract)
class PROJECTWATER_API ASignalEmitter : public AActor
{
	GENERATED_BODY()

	UCharacterMovementComponent* PlayerMovement;
	AWaterPlayerController* PlayerController;

protected:
	// Emitter
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = Emitter, meta = (AllowPrivateAccess = "true"))
	TArray<ASignalReceiver*> Receivers;

	bool bActive;

	// Attention
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = Attention, meta = (AllowPrivateAccess = "true"))
	bool bOnAttention;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = Attention, meta = (AllowPrivateAccess = "true"))
	bool bOffAttention;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = Attention, meta = (AllowPrivateAccess = "true"))
	bool bRepeatAttention;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = Attention, meta = (AllowPrivateAccess = "true", MakeEditWidget = "true"))
	FVector CameraLocation;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = Attention, meta = (AllowPrivateAccess = "true", MakeEditWidget = "true"))
	FVector AttentionLocation;

public:	
	// Sets default values for this actor's properties
	ASignalEmitter();

private:
	void InitializeAttention();

	void PlaySound() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void EmitOnSignal();
	void EmitOffSignal();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ActiveReceivers() const;
	void InactiveReceivers() const;
};
