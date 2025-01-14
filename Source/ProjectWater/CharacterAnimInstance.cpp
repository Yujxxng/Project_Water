// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"
#include "ProjectWaterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Weapon.h"

UCharacterAnimInstance::UCharacterAnimInstance()
{
}

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	Character = Cast<AProjectWaterCharacter>(GetOwningActor());
	if (Character)
	{
		MovementComponent = Character->GetCharacterMovement();
	}
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (MovementComponent)
	{
		Velocity = MovementComponent->Velocity;
		GroundSpeed = Velocity.Size2D();
		ShouldMove = GroundSpeed > 3.0f && MovementComponent->GetCurrentAcceleration() != FVector::ZeroVector;
		IsFalling = MovementComponent->IsFalling();

		HasTool = Character->HasTool();
		IsInteraction = Character->GetInteraction();
	}
}

void UCharacterAnimInstance::AnimNotify_Attack()
{
	//UE_LOG(LogTemp, Log, TEXT("anim notify attack"));

	if (Character->curTool)
	{
		AWeapon* weapon = Cast<AWeapon>(Character->curTool);
		weapon->EnableCollision();
	}

	Character->Attack();
}

void UCharacterAnimInstance::AnimNotify_EndAttack()
{
	if (Character->curTool)
	{
		AWeapon* weapon = Cast<AWeapon>(Character->curTool);
		weapon->DisableCollision();
	}
}
