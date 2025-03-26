// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuoyancyComponent.h"
#include "SwimActorEnterCheck.generated.h"

UCLASS()
class PROJECTWATER_API ASwimActorEnterCheck : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* cubeMeshComponent;
	UStaticMesh* cubeMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBuoyancyComponent* BuoyancyComponent;
public:	
	// Sets default values for this actor's properties
	ASwimActorEnterCheck();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override; 

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	APawn* Player;

	//UFUNCTION()
	void StartSwimming(const FSphericalPontoon& Pontoon);
};
