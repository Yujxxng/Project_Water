// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

// Sets default values
AWeapon::AWeapon() : WeaponMesh(nullptr), Hitbox(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	Hitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox"));
	Hitbox->SetupAttachment(WeaponMesh);
	Hitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Hitbox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnWeaponOverlap);
}

void AWeapon::EnableCollision()
{
	Hitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);	// 공간 정보 사용
}

void AWeapon::DisableCollision()
{
	Hitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::OnWeaponOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ECollisionChannel collisionChannel{ OtherActor->GetComponentByClass<UStaticMeshComponent>()->GetCollisionObjectType() };

	switch (collisionChannel)
	{
	case ECollisionChannel::ECC_GameTraceChannel2:	// Breakable
		UE_LOG(LogTemp, Log, TEXT("breakable destroy"));
		OtherActor->Destroy();
		this->Destroy();
		break;
	}
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

