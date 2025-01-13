// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

class AProjectWaterCharacter;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class PROJECTWATER_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

public:
	UCharacterAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, Category = Character)
	TObjectPtr<AProjectWaterCharacter> Character;

	UPROPERTY(BlueprintReadOnly, Category = Character)
	TObjectPtr<UCharacterMovementComponent> MovementComponent;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	FVector Velocity;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool ShouldMove;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsFalling;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool HasTool;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsInteraction;

private:
	UFUNCTION()
	void AnimNotify_Attack();
};
