// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"
#include "MyCharacter.h"

void UMyAnimInstance::NativeInitializeAnimation()
{

	Super::NativeInitializeAnimation();
	if (MainCharacter == nullptr)
	{
		MainCharacter = Cast<AMyCharacter>(TryGetPawnOwner());
	}
}

void UMyAnimInstance::UpdateAnimationProperties()
{
	// Ensure the Character is valid
	if (MainCharacter == nullptr)
	{
		MainCharacter = Cast<AMyCharacter>(TryGetPawnOwner());
	}


	if (MainCharacter)
	{
		FVector Speed = MainCharacter->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0);

		MovementSpeed = LateralSpeed.Size();


		Direction = UKismetAnimationLibrary::CalculateDirection(Speed, MainCharacter->GetActorRotation());

	}
}
