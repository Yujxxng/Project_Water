// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SignalEmitter.generated.h"

class ASignalReceiver;
class UCharacterMovementComponent;

UCLASS(Abstract)
class PROJECTWATER_API ASignalEmitter : public AActor
{
	GENERATED_BODY()

	TObjectPtr<UCharacterMovementComponent> PlayerMovement;

protected:
	// Emitter
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = Emitter, meta = (AllowPrivateAccess = "true"))
	TArray<ASignalReceiver*> Receivers;

	bool bActivating;
	bool bActivated;

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void EmitOnSignal();
	void EmitOffSignal();

	void ActiveReceivers();
	void InactiveReceivers();

	virtual void PlaySound() PURE_VIRTUAL(ASignalEmitter::PlaySound, );

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
