// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectWaterCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AMoveCharacter

AProjectWaterCharacter::AProjectWaterCharacter() : normalSpeed(500.0f), fasterSpeed(750.0f)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = normalSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	//GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	GetCharacterMovement()->BrakingDecelerationWalking = 0.0f;
	//GetCharacterMovement()->GroundFriction = 0.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AProjectWaterCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

void AProjectWaterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector currentVelocity = GetCharacterMovement()->Velocity;
	FVector horizontalVelocity = FVector(currentVelocity.X, currentVelocity.Y, 0.0f);
	float speed = horizontalVelocity.Size();

	float friction = FMath::Lerp(5.0f, 1.0f, FMath::Clamp(speed / normalSpeed, 0.0f, 1.0f));
	FVector deceleration = -horizontalVelocity.GetSafeNormal() * friction * normalSpeed * 1.5f;

	if (FMath::IsNearlyZero(MoveActionBinding->GetValue().Get<FVector2D>().X) &&
		FMath::IsNearlyZero(MoveActionBinding->GetValue().Get<FVector2D>().Y))
	{
		horizontalVelocity += deceleration * DeltaTime;
		//UE_LOG(LogTemp, Log, TEXT("velocity size :: %f"), GetCharacterMovement()->Velocity.Size());

		if (GetCharacterMovement()->Velocity.Size() < 200.0f)
		{
			horizontalVelocity = FVector::ZeroVector;
		}
	}

	currentVelocity.X = horizontalVelocity.X;
	currentVelocity.Y = horizontalVelocity.Y;

	GetCharacterMovement()->Velocity = currentVelocity;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AProjectWaterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AProjectWaterCharacter::Move);

		// Running
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &AProjectWaterCharacter::StartRun);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AProjectWaterCharacter::EndRun);

		MoveActionBinding = &EnhancedInputComponent->BindActionValue(MoveAction);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AProjectWaterCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AProjectWaterCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AProjectWaterCharacter::StartRun()
{
	GetCharacterMovement()->MaxWalkSpeed = fasterSpeed;
	//UE_LOG(LogTemp, Log, TEXT("start dash"));
}

void AProjectWaterCharacter::EndRun()
{
	GetCharacterMovement()->MaxWalkSpeed = normalSpeed;
	//UE_LOG(LogTemp, Log, TEXT("end dash"));
}

void AProjectWaterCharacter::StartJump()
{
}

void AProjectWaterCharacter::EndJump()
{
}

void AProjectWaterCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}