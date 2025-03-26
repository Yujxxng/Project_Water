// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectWaterCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "WaterBodyComponent.h"
#include "CharacterState.h"
#include "ItemComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AMoveCharacter

AProjectWaterCharacter::AProjectWaterCharacter()
	: InputSubSystem(nullptr)
	, bIgnoreInput(false)
	, PreJumpVelocity()
	, MaxHearts(3), Hearts(MaxHearts)
	, NumKeys(0)
	, MaxOxygen(100.f), Oxygen(MaxOxygen), OxygenUsage(2.f)
	, bOxygenStart(true), OxygenCheckInterval(1000), UseOxygenStart()
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
	CameraBoom->TargetArmLength = 350.f; // The camera follows at this distance behind the character
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	PC = Cast<APlayerController>(GetController());

	// Ignore collision with camera
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	//GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_DescendAscend(TEXT("/Game/ThirdPerson/Input/Actions/IA_DescendAscend"));
	if (IA_DescendAscend.Succeeded())
	{
		DescendAscendAction = IA_DescendAscend.Object;
	}


	// Create a character state
	CharacterState = CreateDefaultSubobject<UCharacterState>(TEXT("CharacterState"));
	CharacterCollector = CreateDefaultSubobject<UItemComponent>(TEXT("Item"));

	//Swim Component
	SwimEnterScene_cpp = CreateDefaultSubobject<USceneComponent>(TEXT("SwimEnterScene_cpp"));
	SwimEnterScene_cpp->SetRelativeLocation(FVector(50.f, 0.f, 70.f));
	SwimBuoyancyScene_cpp = CreateDefaultSubobject<USceneComponent>(TEXT("SwimBuoyancyScene_cpp"));

	Swimming = false;
	SwimSpeed = 250.f;
	MaxSwimVelocity = 500.f;

	Buoyancy_cpp = CreateDefaultSubobject<UBuoyancyComponent>(TEXT("Buoyancy_cpp"));
	FSphericalPontoon Pontoon = FSphericalPontoon();
	Pontoon.CenterSocket = "Pontoon";
	Pontoon.RelativeLocation = FVector(0.f, 0.f, 100.f);
	Pontoon.Radius = 25.f;
	Pontoon.bFXEnabled = true;

	Buoyancy_cpp->BuoyancyData.bCenterPontoonsOnCOM = true;
	Buoyancy_cpp->BuoyancyData.Pontoons.Add(Pontoon);

	////////Load Sound Asset////////
	EnteredWaterSound = LoadObject<USoundWave>(nullptr, TEXT("/Game/Characters/Swimming/Sound/splash-water-103984"));
	ExitWaterSound = LoadObject<USoundWave>(nullptr, TEXT("/Game/Characters/Swimming/Sound/splash-6213"));
	AmbienceSound = LoadObject<USoundWave>(nullptr, TEXT("/Game/Characters/Swimming/Sound/underwater-ambience-6201"));

	//static ConstructorHelpers::FObjectFinder<USoundWave> Ambience(TEXT("/Game/Characters/Swimming/Sound/underwater-ambience-6201"));
	LoadData();
}

void AProjectWaterCharacter::BeginPlay()
{
	JumpMaxHoldTime = 0.f;

	// Call the base class  
	Super::BeginPlay();
	//LoadData();

	if (Buoyancy_cpp)
	{
		FScriptDelegate EnteredWaterDelegate, ExitWaterDelegate;
		EnteredWaterDelegate.BindUFunction(this, FName("PlayEnteredWaterSound"));
		ExitWaterDelegate.BindUFunction(this, FName("PlayExitWaterSound"));

		Buoyancy_cpp->OnEnteredWaterDelegate.Add(EnteredWaterDelegate);
		Buoyancy_cpp->OnExitedWaterDelegate.Add(ExitWaterDelegate);
	}
	ToggleWaterEnterCheck_CPP(true);

}

void AProjectWaterCharacter::SetEnableInput(bool b)
{
	if (!PC)
	{
		PC = Cast<APlayerController>(GetController());
	}

	if (b)
	{
		PC->EnableInput(nullptr);
	}
	else
	{
		PC->DisableInput(nullptr);
	}
}

void AProjectWaterCharacter::SetIgnoreInput(bool b)
{
	bIgnoreInput = b;
}

FInputActionValue AProjectWaterCharacter::GetPlayerInputActionValue() const
{
	return InputSubSystem->GetPlayerInput()->GetActionValue(MoveAction);
}

void AProjectWaterCharacter::HealHearts(int num)
{
	Hearts += num;

	if (Hearts > MaxHearts)
	{
		Hearts = MaxHearts;
	}
}

bool AProjectWaterCharacter::CheckHearts()
{
	if (Hearts < 0)
	{
		UE_LOG(LogTemp, Log, TEXT("!!!!! GAME OVER !!!!!"));
		return false;
	}
	return true;
}

float AProjectWaterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//Hearts -= int(damage);
	Hearts -= 1;
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	UE_LOG(LogTemp, Log, TEXT("damage %f Hearts %d"), damage, Hearts);

	CheckHearts();

	return damage;
}

void AProjectWaterCharacter::GetKey()
{
	NumKeys++;
}

bool AProjectWaterCharacter::UseKey()
{
	if (NumKeys > 0)
	{
		NumKeys--;
		return true;
	}

	return false;
}

void AProjectWaterCharacter::RecoverOxygen()
{
	bOxygenStart = true;
	Oxygen = MaxOxygen;
}

bool AProjectWaterCharacter::UseOxygen()
{
	static std::chrono::duration<long, std::milli> adder{ 0 };

	if (bOxygenStart)
	{
		UseOxygenStart = std::chrono::system_clock::now();
		bOxygenStart = false;
	}

	std::chrono::system_clock::time_point cur = std::chrono::system_clock::now();
	adder += std::chrono::duration_cast<std::chrono::milliseconds>(cur - UseOxygenStart);
	if (adder >= OxygenCheckInterval)
	{
		UseOxygenStart = cur;
	}

	while (adder >= OxygenCheckInterval)
	{
		Oxygen -= OxygenUsage;
		adder -= OxygenCheckInterval;

		UE_LOG(LogTemp, Log, TEXT("Oxygen %f"), Oxygen);
	}

	if (Oxygen <= 0.f)
	{
		//Hearts -= 1;
		UGameplayStatics::ApplyDamage(this, 1.f, this->GetInstigatorController(), nullptr, NULL);
		Oxygen += OxygenUsage * 1000.0 / OxygenCheckInterval.count() * 10;	// 10 seconds

		if (!CheckHearts())
		{
			return false;
		}
	}

	return true;
}

void AProjectWaterCharacter::LoadData()
{
	UWaterGameInstance* GameInstance = Cast<UWaterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		Hearts = GameInstance->HeartNum;
		UE_LOG(LogTemp, Warning, TEXT("Hearts Load"));
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Hearts Load fail"));
}

void AProjectWaterCharacter::MoveBlueprintTemp(FVector2D Value)
{
	if (Controller != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Value %f %f"), Value.X, Value.Y);

		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Value.Y);
		AddMovementInput(RightDirection, Value.X);
	}
}

void AProjectWaterCharacter::PlayEnteredWaterSound(const FSphericalPontoon& Pontoon)
{
	if (EnteredWaterSound)
	{
		//UE_LOG(LogTemp, Warning, TEXT("EnteredWaterSound"));
		float Z = GetVelocity().Z;
		float VolumeMultiplier = MapRangeClamped(FMath::Abs(Z), 500.f, 400.f, 0.1f, 1.f);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), EnteredWaterSound, GetActorLocation(), VolumeMultiplier, FMath::FRandRange(0.9f, 1.1f));
	}
}

void AProjectWaterCharacter::PlayExitWaterSound(const FSphericalPontoon& Pontoon)
{
	if (ExitWaterSound)
	{
		UE_LOG(LogTemp, Warning, TEXT("ExitWaterSound"));
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExitWaterSound, GetActorLocation(), 0.8f, FMath::FRandRange(0.9f, 1.1f));
	}
}

void AProjectWaterCharacter::ToggleWaterEnterCheck_CPP(bool Enabled)
{
	//Either spawn or destroy the enter check actor.
	if (Enabled)
	{
		if (SwimEnterScene_cpp->GetNumChildrenComponents() > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Spawn Actor"));
			SwimEnterScene_cpp->GetChildComponent(0)->DestroyComponent();

			FTransform NewTransform;
			NewTransform.SetScale3D(FVector(0.2f, 0.2f, 0.2f));
			SwimActorEnterCheck = GetWorld()->SpawnActorDeferred<ASwimActorEnterCheck>(ASwimActorEnterCheck::StaticClass(), NewTransform, this, nullptr);
		
			SwimActorEnterCheck->AttachToComponent(SwimEnterScene_cpp, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, true));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Destroy Actor"));
		SwimEnterScene_cpp->GetChildComponent(0)->GetOwner()->Destroy();
	}
}

void AProjectWaterCharacter::ToggleUnderwaterAmbience_CPP(bool Enabled)
{
	//Control whether the underwater ambience is currently playing. using fades to make it blend in and out.
	if (Enabled)
	{
		if(!IsValid(UnderwaterAmbience))
		{
			UnderwaterAmbience = UGameplayStatics::SpawnSoundAttached(AmbienceSound, this->GetMesh());
			UnderwaterAmbience->FadeIn(0.2f);
		}
	}
	else
	{
		if (IsValid(UnderwaterAmbience))
		{
			UnderwaterAmbience->FadeOut(0.4f, 0.f);
		}
	}
}

void AProjectWaterCharacter::SwimmingBuoyancy_CPP(float SurfaceBuoyancyZCorrection, float InterpSpeed)
{
	//When the player is at the water surface, since the character is not a physics object, the buoyancy component won't make them move with the waves, so this uses the buoyancy actor to fake it.
	switch (SwimMode_CPP)
	{
	case SwimMode_Enum::Treading:
	case SwimMode_Enum::SurfaceSwimming:
		if (InputSubSystem->GetPlayerInput()->GetActionValue(DescendAscendAction).Get<float>() > 1.f)
		{
			if (IsValid(SwimBuoyancyActor))
			{
				float A = SwimBuoyancyActor->cubeMeshComponent->GetComponentLocation().Z;
		
				bool bSelectA = GetVelocity().Length() > 2.0;
				float B = UKismetMathLibrary::SelectFloat(0.5f, 1.0f, bSelectA) * SurfaceBuoyancyZCorrection;
				
				
				SetActorLocation(UKismetMathLibrary::VInterpTo(GetActorLocation(), FVector(GetActorLocation().X, GetActorLocation().Y, (A - B)), GetWorld()->GetDeltaSeconds(), InterpSpeed));
			}
		}
		break;
	case SwimMode_Enum::UnderwaterIdle:
		break;
	case SwimMode_Enum::UnderwaterSwimming:
		break;
	default:
		break;
	}
}

void AProjectWaterCharacter::ExitSwimTrace_CPP(float UpOffset, float DownOffset, float MaxDepth)
{
	FHitResult HitResult;
	FVector Start = GetMesh()->GetSocketLocation(FName("head")) + FVector(10.f, 10.f, UpOffset);
	FVector End = GetMesh()->GetSocketLocation(FName("head")) - FVector(10.f, 10.f, DownOffset);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	//UE_LOG(LogTemp, Warning, TEXT("Sphere Trace"));
	if (Buoyancy_cpp->GetCurrentWaterBodyComponents().IsEmpty())
	{
		if (Buoyancy_cpp->GetCurrentWaterBodyComponents().IsValidIndex(0))
		{
			auto WaterBody = Buoyancy_cpp->GetCurrentWaterBodyComponents().GetData()->Get();
			FVector VTemp;
			float OutWaterDepth;
			WaterBody->GetWaterSurfaceInfoAtLocation(GetActorLocation(), VTemp, VTemp, VTemp, OutWaterDepth, true);
			bool bTrace = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(25.f), Params);
			if (bTrace && (OutWaterDepth < MaxDepth))
			{
				//If the player close to the ground and the water depth is low enough (meaning there is air above them), then swimming will end.
				EndSwimming();
			}
			else {}
		}

	}

}

void AProjectWaterCharacter::EventStartSwimming_CPP()
{
	//Start swimming and spawn buoyancy object.
	if (!Swimming)
	{
		Swimming = true;
		ToggleWaterEnterCheck_CPP(false);
		CMC->SetMovementMode(MOVE_Flying);
		CMC->MaxFlySpeed = SwimSpeed;
		CMC->bOrientRotationToMovement = false;
		ToggleUnderwaterAmbience_CPP(true);

		SwimHorizontal = UKismetMathLibrary::MakeRotFromX(GetActorForwardVector());
		
		FTransform NewTransform;
		NewTransform.SetLocation(SwimEnterScene_cpp->GetComponentLocation());
		SwimBuoyancyActor = GetWorld()->SpawnActorDeferred<ASwimmingBuoyancy>(ASwimmingBuoyancy::StaticClass(), NewTransform, nullptr, this);

		GetWorldTimerManager().SetTimer(SwimmingTimerHandle, this, &AProjectWaterCharacter::SwimmingTick, 0.01f, true, 0.f);
	}
}

void AProjectWaterCharacter::SwimmingTick()
{

	//Timer that sets the swim mode value, calls the function and limits the max velocity (so if the player enters the water with a higher velocity, it won't float at a fast speed.
	//Sequence 0
	bool bIndex = GetVelocity().Length() > 10.f;
	if (Buoyancy_cpp->IsInWaterBody())
	{
		if (bIndex)
			SwimMode_CPP = SwimMode_Enum::UnderwaterSwimming;
		else
			SwimMode_CPP = SwimMode_Enum::UnderwaterIdle;
	}
	else
	{
		if (bIndex)
			SwimMode_CPP = SwimMode_Enum::SurfaceSwimming;
		else
			SwimMode_CPP = SwimMode_Enum::Treading;
	}
	SwimBuoyancyActor->SetXYPosition();


	//Sequence 1
	SwimmingBuoyancy_CPP();

	//Sequence 2
	ExitSwimTrace_CPP();

	//Sequence 3
	ToggleUnderwaterAmbience_CPP(SwimMode_CPP != SwimMode_Enum::Treading);
	if (GetVelocity().Length() > MaxSwimVelocity)
	{
		CMC->Velocity = GetVelocity().GetClampedToSize(0.0f, MaxSwimVelocity);
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
			InputSubSystem = Subsystem;
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

		// Interacting
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AProjectWaterCharacter::Interact);

		EnhancedInputComponent->BindAction(DescendAscendAction, ETriggerEvent::Triggered, this, &AProjectWaterCharacter::DescendAscend);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AProjectWaterCharacter::Jump()
{
	PreJumpVelocity = CMC->Velocity;
	PreJumpVelocity.Z = 0.f;

	FInputActionValue moveValue = InputSubSystem->GetPlayerInput()->GetActionValue(MoveAction);
	if (moveValue.IsNonZero() || CharacterState->GetExhausted())
	{
		PreJumpVelocity.Z = CMC->JumpZVelocity;

		if (moveValue.IsNonZero())
		{
			CMC->JumpZVelocity *= 1.2f;
		}

		if (CharacterState->GetExhausted())
		{
			CMC->JumpZVelocity *= 0.4f;
		}
	}

	Super::Jump();
}

void AProjectWaterCharacter::StopJumping()
{
	Super::StopJumping();

	switch (CharacterState->GetState())
	{
	case EState::STATE_Vapor:
	case EState::STATE_Ice:
		CMC->Velocity.X = PreJumpVelocity.X;
		CMC->Velocity.Y = PreJumpVelocity.Y;

	default:
		CMC->Velocity.X = PreJumpVelocity.X * 0.6f;
		CMC->Velocity.Y = PreJumpVelocity.Y * 0.6f;
	}

	if (PreJumpVelocity.Z >= 0.00001f)
	{
		CMC->JumpZVelocity = PreJumpVelocity.Z;
	}

	FInputActionValue moveValue = InputSubSystem->GetPlayerInput()->GetActionValue(MoveAction);
	if (moveValue.IsNonZero())
	{
		Move(moveValue);
	}
}

void AProjectWaterCharacter::Move(const FInputActionValue& Value)
{
	if (bIgnoreInput)
	{
		return;
	}

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
	if (bIgnoreInput)
	{
		return;
	}

	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AProjectWaterCharacter::DescendAscend(const FInputActionValue& Value)
{
	//Value = Axis1D(Float)

	UE_LOG(LogTemp, Warning, TEXT("AProjectWaterCharacter DescendAscend"));
}

void AProjectWaterCharacter::Interact()
{
	//UE_LOG(LogTemp, Log, TEXT("AProjectWaterCharacter Interact"));

	TArray<AActor*> overlappingActors;
	GetOverlappingActors(overlappingActors);

	for (AActor* actor : overlappingActors)
	{
		IInteract* interactVar = Cast<IInteract>(actor);
		if (interactVar)
		{
			interactVar->Interact();
			break;
		}
	}
}
