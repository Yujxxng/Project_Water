// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Fan.generated.h"

class UCapsuleComponent;
class UHierarchicalInstancedStaticMeshComponent;

UCLASS()
class PROJECTWATER_API AFan : public AActor
{
	GENERATED_BODY()

	// 컴포넌트
	UHierarchicalInstancedStaticMeshComponent* HISM;

	// 변수
	UStaticMesh* SM;

public:	
	// Sets default values for this actor's properties
	AFan();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
