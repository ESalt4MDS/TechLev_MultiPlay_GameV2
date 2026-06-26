// Copyright Epic Games, Inc. All Rights Reserved.

#include "NetProjectGameCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "NetProjectGame.h"
#include "NPG_Cube.h"
#include "NPG_Projectile.h"
#include "Net/UnrealNetwork.h"
#include "NetProjectGamePlayerController.h"
#include "NPG_PlayerState.h"
#include "NPG_InteractableDoor.h"

ANetProjectGameCharacter::ANetProjectGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ANetProjectGameCharacter::Tick(float _DelataTime)
{
	Super::Tick(_DelataTime);

	if (HasAuthority())
	{
		ControlPitch = GetControlRotation().Pitch;
	}

	/*
	if (IsLocallyControlled())
	{
		if (HasAuthority())
		{
			GEngine->AddOnScreenDebugMessage(1, 0.1f, FColor::Green, "Server: " + FString::FromInt(CubesRemaining), true, FVector2D(4,4));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(2, 0.1f, FColor::Red, "Client: " + FString::FromInt(CubesRemaining), true, FVector2D(4, 4));

		}
	}
	*/
}

float ANetProjectGameCharacter::GetReplicatedPitch()
{
	if (IsLocallyControlled())
	{
		return GetControlRotation().Pitch;
	}
	return ControlPitch;
}

void ANetProjectGameCharacter::NPG_TakeDamage(float _Damage, ANPG_PlayerState* _PlayerThatDealtDamage)
{
	Health -= _Damage;

	if (Health <= 0.0f)
	{
		Die(_PlayerThatDealtDamage);
	}

}

void ANetProjectGameCharacter::ToggleDoor_Implementation(ANPG_InteractableDoor* _Door)
{
	_Door->ToggleDoor();
}

/* shoot a line trace to check if can interact with a door object */
void ANetProjectGameCharacter::InteractTrace()
{
	//line trace variables
	FVector CameraWorldLocation = FollowCamera->GetComponentLocation();
	FVector ForwardVector = FollowCamera->GetForwardVector();

	FVector TraceStart = CameraWorldLocation;
	FVector TraceEnd = CameraWorldLocation + (ForwardVector * InteractRange);

	FHitResult InteractHitResult;

	//line trace
	GetWorld()->LineTraceSingleByChannel(InteractHitResult, TraceStart, TraceEnd, ECC_Visibility);

	//check if hit an actor
	if (InteractHitResult.GetActor())
	{	
		ANPG_InteractableDoor* InteractableDoor = Cast<ANPG_InteractableDoor>(InteractHitResult.GetActor());

		if (InteractableDoor)
		{
			InteractTarget = InteractHitResult.GetActor();

			ToggleDoor(InteractableDoor);
		}
		else
		{
			InteractTarget = nullptr;
		}
	}
}

void ANetProjectGameCharacter::Die(ANPG_PlayerState* _PlayerThatEliminatedYou)
{
	GetController<ANetProjectGamePlayerController>()->RespawnAfterDelay();

	if (_PlayerThatEliminatedYou)
	{
		_PlayerThatEliminatedYou->GiveElimination();
	}

	Destroy();
}

void ANetProjectGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ANetProjectGameCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ANetProjectGameCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ANetProjectGameCharacter::Look);

		//Interacting
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ANetProjectGameCharacter::Interact);
		
		//Attack
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ANetProjectGameCharacter::Attack);

	}
	else
	{
		UE_LOG(LogNetProjectGame, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ANetProjectGameCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void ANetProjectGameCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ANetProjectGameCharacter::Attack(const FInputActionValue& Value)
{
	ServerAttack();
}

void ANetProjectGameCharacter::Interact(const FInputActionValue& Value)
{
	InteractTrace();
	//ServerSpawnCube();
}


void ANetProjectGameCharacter::ServerAttack_Implementation()
{	
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(GetMesh()->GetBoneLocation("hand_l"));
	SpawnTransform.SetRotation(GetControlRotation().Quaternion());

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	ANPG_Projectile* SpawnedProjectile = GetWorld()->SpawnActor<ANPG_Projectile>(ProjectileClass, SpawnTransform, SpawnParams);
}

void ANetProjectGameCharacter::ServerSpawnCube_Implementation()
{
	if (CubesRemaining <= 0) return;
	CubesRemaining--;

	FTransform SpawnTransform(GetActorLocation() + GetActorForwardVector() * 100.0f); // 1 meter infront of player

	ANPG_Cube* SpawnedCube = GetWorld()->SpawnActor<ANPG_Cube>(CubeClass, SpawnTransform);
}

void ANetProjectGameCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void ANetProjectGameCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ANetProjectGameCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void ANetProjectGameCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void ANetProjectGameCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME (ANetProjectGameCharacter, CubesRemaining);
	DOREPLIFETIME(ANetProjectGameCharacter, ControlPitch);
	DOREPLIFETIME(ANetProjectGameCharacter, Health);

}
