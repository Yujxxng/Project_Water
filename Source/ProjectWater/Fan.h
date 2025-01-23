// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Fan.generated.h"

class UCapsuleComponent;

UCLASS()
class PROJECTWATER_API AFan : public AActor
{
	GENERATED_BODY()
	

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CollisionComponent;

	UPROPERTY(EditAnywhere)
	float FanForce;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
public:	
	// Sets default values for this actor's properties
	AFan();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
