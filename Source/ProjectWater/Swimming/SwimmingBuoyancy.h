// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuoyancyComponent.h"
#include "SwimmingBuoyancy.generated.h"

UCLASS()
class PROJECTWATER_API ASwimmingBuoyancy : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* cubeMeshComponent;
	UStaticMesh* cubeMesh;

public:	
	// Sets default values for this actor's properties
	ASwimmingBuoyancy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
