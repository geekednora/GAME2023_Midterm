// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"



// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate (Yaw)
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	// Create camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon")); // Create the weapon
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMyCharacter::OnConstruction(const FTransform& Transform)
{
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("WeaponSocket"));
}

void AMyCharacter::Attack()
{
	const float kLineTraceRange = 10000.0f;

	// get the camera view
	FVector CameraLocation = Weapon->GetComponentLocation();
	FRotator CameraRotation = FollowCamera->GetComponentRotation();

	// get the end point of the line trace
	FVector Start = FVector(CameraLocation.X + 50.0f, CameraLocation.Y, CameraLocation.Z - 10.0f);
	FVector End = Start + (CameraRotation.Vector() * kLineTraceRange);

	// trace parameters
	FCollisionQueryParams TraceParams(FName(TEXT("InteractTrace")), true, NULL);
	TraceParams.bTraceComplex = false;

	FHitResult HitData = FHitResult(ForceInit);

	// trace!
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitData, Start, End, ECC_Camera, TraceParams);

	if (bHit)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Hit"));
		DrawDebugLine(GetWorld(), Start, End, FColor::Green, ECC_WorldStatic, 5.0f);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Actor name: %s"), *HitData.GetActor()->GetName()));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Impact Point: %s"), *HitData.ImpactPoint.ToString()));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Distance: %s"), *FString::SanitizeFloat(HitData.Distance)));
		DrawDebugBox(GetWorld(), HitData.ImpactPoint, FVector(2.f, 2.f, 2.f), FColor::Red, ECC_WorldStatic, 1.0f);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Miss"));
		DrawDebugLine(GetWorld(), Start, End, FColor::Purple, ECC_WorldStatic, 1.0f);
	}
}