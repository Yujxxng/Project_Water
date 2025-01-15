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

#include "Weapon.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AMoveCharacter

AProjectWaterCharacter::AProjectWaterCharacter()
	: normalSpeed(500.0f), fasterSpeed(750.0f), jumpMaxHoldTime(0.25f)
	, bInteraction(false), curTool(nullptr)
{
	MovementComponent = GetCharacterMovement();

	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	MovementComponent->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	MovementComponent->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	MovementComponent->AirControl = 0.35f;
	MovementComponent->MaxWalkSpeed = normalSpeed;
	MovementComponent->MinAnalogWalkSpeed = 20.f;
	MovementComponent->BrakingDecelerationWalking = 2000.0f;
	MovementComponent->BrakingDecelerationFalling = 1500.0f;

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


	JumpMaxHoldTime = jumpMaxHoldTime;

	// Ignore collision with camera
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

void AProjectWaterCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

void AProjectWaterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Log, TEXT("movement mode :: %d"), GetCharacterMovement()->MovementMode);

	switch (characterPhase)
	{
	case ECharacterPhase::VAPOR_PHASE:
		JumpMaxHoldTime = 5.f;
		break;

	default:
		JumpMaxHoldTime = jumpMaxHoldTime;
	}
}

void AProjectWaterCharacter::SetCharacterPhase(ECharacterPhase newPhase)
{
	if (characterPhase == newPhase)
		return;

	characterPhase = newPhase;

	switch (newPhase)
	{
	case ECharacterPhase::HUMAN_PHASE:
		MovementComponent->AirControl = 0.35f;
		MovementComponent->BrakingDecelerationWalking = 2000.0f;
		MovementComponent->GroundFriction = 8.0f;
		MovementComponent->GravityScale = 1.75f;
		MovementComponent->Mass = 100.f;
		break;

	case ECharacterPhase::VAPOR_PHASE:
		MovementComponent->AirControl = 0.8f;
		MovementComponent->BrakingDecelerationWalking = 2000.0f;
		MovementComponent->GroundFriction = 8.0f;
		MovementComponent->GravityScale = 0.25f;
		MovementComponent->Mass = 5.f;
		break;

	case ECharacterPhase::ICE_PHASE:
		MovementComponent->AirControl = 0.35f;
		MovementComponent->BrakingDecelerationWalking = 0.0f;
		MovementComponent->GroundFriction = 0.0f;
		MovementComponent->GravityScale = 1.75f;
		MovementComponent->Mass = 100.f;
		break;
	}
}

void AProjectWaterCharacter::GetTool(AActor* tool)
{
	curTool = tool;
	tool->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("hand_r_socket"));

}

void AProjectWaterCharacter::NotifyActorBeginOverlap(AActor* OtherActor)
{
	ECollisionChannel collisionChannel{ OtherActor->GetComponentByClass<UStaticMeshComponent>()->GetCollisionObjectType() };

	switch (collisionChannel)
	{
	case ECollisionChannel::ECC_GameTraceChannel1:	// TOOLS
		GetTool(OtherActor);
		break;
	}
}

void AProjectWaterCharacter::Attack()
{
	//UE_LOG(LogTemp, Log, TEXT("character attack"));

	if (curTool && !IsValid(curTool))
	{
		curTool = nullptr;
	}
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

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AProjectWaterCharacter::Look);

		// Interaction
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AProjectWaterCharacter::SetInteraction);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &AProjectWaterCharacter::SetInteraction);
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
	MovementComponent->MaxWalkSpeed = fasterSpeed;
	MovementComponent->MaxFlySpeed = fasterSpeed;
	//UE_LOG(LogTemp, Log, TEXT("start dash"));
}

void AProjectWaterCharacter::EndRun()
{
	MovementComponent->MaxWalkSpeed = normalSpeed;
	MovementComponent->MaxFlySpeed = normalSpeed;
	//UE_LOG(LogTemp, Log, TEXT("end dash"));
}

void AProjectWaterCharacter::StartFlyUp()
{
	if (characterPhase == ECharacterPhase::VAPOR_PHASE)
	{
		UE_LOG(LogTemp, Log, TEXT("start fly up"));
		MovementComponent->AddImpulse(GetActorForwardVector() * 100.f, true);
	}
}

void AProjectWaterCharacter::FlyUp()
{
	if (characterPhase == ECharacterPhase::VAPOR_PHASE)
	{
		UE_LOG(LogTemp, Log, TEXT("fly up"));
		//AddMovementInput(GetActorUpVector(), 1.0f);
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

void AProjectWaterCharacter::SetInteraction()
{
	bInteraction = !bInteraction;
}

void AProjectWaterCharacter::AnimNotify_Attack()
{
	//UE_LOG(LogTemp, Log, TEXT("anim notify attack"));

	if (curTool)
	{
		AWeapon* weapon = Cast<AWeapon>(curTool);
		weapon->EnableCollision();
	}

	Attack();
}

void AProjectWaterCharacter::AnimNotify_EndAttack()
{
	if (curTool)
	{
		AWeapon* weapon = Cast<AWeapon>(curTool);
		weapon->DisableCollision();
	}
}
