// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Basic_Unreal4_CPPCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// ABasic_Unreal4_CPPCharacter

ABasic_Unreal4_CPPCharacter::ABasic_Unreal4_CPPCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ABasic_Unreal4_CPPCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABasic_Unreal4_CPPCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABasic_Unreal4_CPPCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ABasic_Unreal4_CPPCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ABasic_Unreal4_CPPCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ABasic_Unreal4_CPPCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ABasic_Unreal4_CPPCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ABasic_Unreal4_CPPCharacter::OnResetVR);
}


void ABasic_Unreal4_CPPCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ABasic_Unreal4_CPPCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ABasic_Unreal4_CPPCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ABasic_Unreal4_CPPCharacter::Jump()
{
	if (CanJump())
	{
		HowManyJumps = 0;
		//Call the Character Jump Function
		Super::Jump();

		UE_LOG(LogTemp, Warning, TEXT("Player can Jump!!!"));
		HowManyJumps++;
	}
	else if (!CanJump() && HowManyJumps == 1)
	{
		//Show in Unreal LOG the message
		UE_LOG(LogTemp, Warning, TEXT("Player is Jumping!!!"));
		//Increment the number of jumps
		HowManyJumps++;
		//Increment the Height of Jump to double
		GetCharacterMovement()->Velocity += FVector(0, 0, 1) * GetCharacterMovement()->JumpZVelocity;
	}

	FString JumpText = FString::Printf(TEXT("The Player has jumped %d times...."), HowManyJumps);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *JumpText);

	/*
	int  * JumpHeight;												//Declare a pointer variable for the Jump Height
	JumpJeight = new int();	// (int*)malloc(sizeof(int));			//Allocates the memory for this pointer
	*JumpHeight = 10;												//Assingns the content of this pointer

	//Here we log this value of Jump Height
	FString jumpHeightText = FString::Printf(TEXT("The player can jamp %d meters up in the air."), *JumpHeight);
	UE_LOG(LogTemp, Warning, TEXT(%s), *jumpHeightText);

	delete JumpHeight;

	JumpHeight = 0;
	*/
}

void ABasic_Unreal4_CPPCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ABasic_Unreal4_CPPCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ABasic_Unreal4_CPPCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ABasic_Unreal4_CPPCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
