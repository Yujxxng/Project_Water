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

#include "CharacterState.h"
#include "Engine/DamageEvents.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AMoveCharacter

AProjectWaterCharacter::AProjectWaterCharacter()
	: PreJumpVelocity()
	, MaxHearts(3), Hearts(MaxHearts)
	, numKeys(0)
{	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(35.f, 90.f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	CMC = GetCharacterMovement();
	CMC->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	CMC->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	CMC->MinAnalogWalkSpeed = 20.f;
	CMC->JumpZVelocity = 750.f;

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


	// Ignore collision with camera
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	//GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	
	// Create a character state
	CharacterState = CreateDefaultSubobject<UCharacterState>(TEXT("CharacterState"));
}

void AProjectWaterCharacter::BeginPlay()
{
	JumpMaxHoldTime = 0.f;

	// Call the base class  
	Super::BeginPlay();
}

//void AProjectWaterCharacter::DamageHearts(int num)
//{
//	Hearts -= num;
//
//	if (Hearts < 0)
//	{
//		UE_LOG(LogTemp, Log, TEXT("!!!!! GAME OVER !!!!!"));
//	}
//}

void AProjectWaterCharacter::Jump()
{
	PreJumpVelocity = CMC->Velocity;
	PreJumpVelocity.Z = 0.f;
	if (PreJumpVelocity.X > 0.1f || PreJumpVelocity.Y > 0.1f)
	{
		PreJumpVelocity.Z = CMC->JumpZVelocity;
		CMC->JumpZVelocity *= 1.2f;
	}

	Super::Jump();
}

void AProjectWaterCharacter::StopJumping()
{
	Super::StopJumping();

	CMC->Velocity.X = PreJumpVelocity.X * 0.5f;
	CMC->Velocity.Y = PreJumpVelocity.Y * 0.5f;

	if (PreJumpVelocity.Z >= 0.00001f)
	{
		CMC->JumpZVelocity = PreJumpVelocity.Z;
	}
}

void AProjectWaterCharacter::HealHearts(int num)
{
	Hearts += num;

	if (Hearts > MaxHearts)
	{
		Hearts = MaxHearts;
	}
}

float AProjectWaterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// PointDamage
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		Hearts -= int(damage);
	}

	if (Hearts < 0)
	{
		UE_LOG(LogTemp, Log, TEXT("!!!!! GAME OVER !!!!!"));
	}

	return damage;
}

void AProjectWaterCharacter::GetKey()
{
	numKeys++;
}

bool AProjectWaterCharacter::UseKey()
{
	if (numKeys > 0)
	{
		numKeys--;
		return true;
	}

	return false;
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
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AProjectWaterCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AProjectWaterCharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AProjectWaterCharacter::Move);

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