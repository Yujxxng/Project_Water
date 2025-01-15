// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "ProjectWaterCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UENUM(BlueprintType)
enum class ECharacterPhase : uint8
{
	HUMAN_PHASE	UMETA(DisplayName = "Human phase"),
	WATER_PHASE	UMETA(DisplayName = "Water phase"),
	VAPOR_PHASE	UMETA(DisplayName = "Vapor phase"),
	ICE_PHASE	UMETA(DisplayName = "Ice phase")
};

UCLASS(config = Game)
class AProjectWaterCharacter : public ACharacter
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

	/** Run Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RunAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Interact(Yejin) Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

public:
	AProjectWaterCharacter();


protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	void StartRun();
	void EndRun();

	void StartFlyUp();
	void FlyUp();

	/** Called for looking input */
	void Look(const FInputActionValue& Value);


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


private:
	TObjectPtr<UCharacterMovementComponent> MovementComponent;

	float normalSpeed;
	float fasterSpeed;

	float jumpMaxHoldTime;

	UPROPERTY(VisibleDefaultsOnly, BlueprintSetter = SetCharacterPhase)
	ECharacterPhase characterPhase;
	UFUNCTION(BlueprintSetter)
	void SetCharacterPhase(ECharacterPhase newPhase);

	bool bInteraction;
	void SetInteraction();

	UFUNCTION(BlueprintCallable)
	void GetTool(AActor* tool);

protected:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:
	AActor* curTool;
	UFUNCTION(BlueprintCallable)
	bool HasTool() const { return curTool != nullptr; }
	UFUNCTION(BlueprintCallable)
	bool GetInteraction() const { return bInteraction; }
	UFUNCTION(BlueprintCallable)
	void AnimNotify_Attack();

	UFUNCTION(BlueprintCallable)
	void AnimNotify_EndAttack();
	void Attack();
};

