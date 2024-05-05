// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyCharacter.h"




void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultInputMappingContext, 0);
	}

	ACharacter* ControlledCharacter = GetCharacter();
	if (ControlledCharacter)
	{
		ControlledCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Setup action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{	
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Look);

		// Shooting
EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Shoot);
	}
}

void AMyPlayerController::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	APawn* ControlledPawn = GetPawn();


	if (ACharacter* ControlledCharacter = GetCharacter())
	{
		// Moving backwards, left, or right -> use strafe movement
		if (MovementVector.X < 0 || MovementVector.Y != 0)
		{
			// Strafe movement - player does not rotate to the movement
			ControlledCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;

			// Add Yaw and Pitch input to controller
			ControlledCharacter->AddMovementInput(ControlledCharacter->GetActorForwardVector(), MovementVector.X);
			ControlledCharacter->AddMovementInput(ControlledCharacter->GetActorRightVector(), MovementVector.Y * -1);
		}
		else
		{
			// Movement towards camera forward vector
			ControlledCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;

			// Find out which direction's camera is facing
			const FRotator Rotation = GetControlRotation();
			const FRotator CameraRotation(0, Rotation.Yaw, 0);

			// Get Forward Vector
			const FVector ForwardDirection = FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::X);
			// Add movement in that direction
			ControlledCharacter->AddMovementInput(ForwardDirection, MovementVector.X);

			// Get Right Vector
			const FVector RightDirection = FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::Y);
			// Add movement in that direction
			ControlledCharacter->AddMovementInput(RightDirection, MovementVector.Y);
		}
	}
}

void AMyPlayerController::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddYawInput(LookAxisVector.X);
	AddPitchInput(-LookAxisVector.Y);
}

void AMyPlayerController::Shoot(const FInputActionValue& Value)
{
	if(AMyCharacter* ControlledCharacter = Cast<AMyCharacter>(GetCharacter()))
	{
		ControlledCharacter->Attack();
	}
}

