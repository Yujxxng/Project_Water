// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "WaterGameInstance.h"
#include "BuoyancyComponent.h"
#include <chrono>
#include "PuzzleSystem/Interact.h"
#include "ProjectWaterCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

class UCharacterState;
class UItemComponent;
class UEnhancedInputLocalPlayerSubsystem;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config = Game)
class AProjectWaterCharacter : public ACharacter, public IInteract
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

public:
	AProjectWaterCharacter();


protected:

	/** Called for movement input */
	UFUNCTION(BlueprintCallable)
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Interact() override;


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


	//////////////////////////////////////////////////////////////////////////
	// Custom
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Swim, meta = (AllowPrivateAccess = "true"))
	USceneComponent* SwimEnterScene_cpp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Swim, meta = (AllowPrivateAccess = "true"))
	USceneComponent* SwimBuoyancyScene_cpp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Swim, meta = (AllowPrivateAccess = "true"))
	UBuoyancyComponent* Buoyancy_cpp;

private:
	TObjectPtr<UCharacterMovementComponent> CMC;

	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> InputSubSystem;
	TObjectPtr<APlayerController> PC;

	UPROPERTY(VisibleDefaultsOnly, BlueprintSetter = SetIgnoreInput)
	bool bIgnoreInput;

	FVector PreJumpVelocity;

	int MaxHearts;
	UPROPERTY(VisibleDefaultsOnly, BlueprintGetter = GetHearts)
	int Hearts;

	UPROPERTY(VisibleDefaultsOnly)
	int NumKeys;

	float MaxOxygen;
	UPROPERTY(VisibleDefaultsOnly, BlueprintGetter = GetOxygen)
	float Oxygen;
	float OxygenUsage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintGetter = GetOxygenStart)
	bool bOxygenStart;
	const std::chrono::duration<long, std::milli> OxygenCheckInterval;
	std::chrono::system_clock::time_point UseOxygenStart;

public:
	virtual void Jump() override;
	virtual void StopJumping() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "CharacterState", meta = (AllowPrivateAccess = "true"))
	UCharacterState* CharacterState;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	UItemComponent* CharacterCollector;

	ACameraActor* AttentionCamera;

	UFUNCTION(BlueprintCallable)
	void SetEnableInput(bool b);

	UFUNCTION(BlueprintSetter)
	void SetIgnoreInput(bool b);

	FInputActionValue GetPlayerInputActionValue() const;

	UFUNCTION(BlueprintCallable)
	int GetMaxHearts() const { return MaxHearts; }
	UFUNCTION(BlueprintCallable)
	int GetHearts() const { return Hearts; }

	UFUNCTION(BlueprintCallable)
	void HealHearts(int num = 1);

	UFUNCTION(BlueprintCallable)
	bool CheckHearts();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	void GetKey();

	UFUNCTION(BlueprintCallable)
	bool UseKey();

	UFUNCTION(BlueprintCallable)
	void RecoverOxygen();

	UFUNCTION(BlueprintGetter)
	float GetOxygen() const { return Oxygen; }

	UFUNCTION(BlueprintGetter)
	bool GetOxygenStart() const { return bOxygenStart; }

	UFUNCTION(BlueprintCallable)
	bool UseOxygen();

	UFUNCTION(BlueprintCallable)
	void LoadData();

	/* TEMP FUNCTION */
	UFUNCTION(BlueprintCallable)
	void MoveBlueprintTemp(FVector2D Value);
};

