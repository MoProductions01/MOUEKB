// Fill out your copyright notice in the Description page of Project Settings.


#include "Human.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Called every frame
void AHuman::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	

	// This is the tweak to make sure that the collision physics will work even if the actor isn't moving (normally
	// physics is ignored if the actor is still.)
	FHitResult Hit;
	FVector MoveAdj = FVector(1.f, 0.f, 0.f);	
	// bool UMovementComponent::SafeMoveUpdatedComponent(const FVector& Delta, const FRotator& NewRotation, bool bSweep, FHitResult& OutHit, ETeleportType Teleport)
	GetCharacterMovement()->SafeMoveUpdatedComponent(MoveAdj, GetActorRotation(), true, Hit);
	GetCharacterMovement()->SafeMoveUpdatedComponent(-MoveAdj, GetActorRotation(), true, Hit);
	
	// NOTE: this below doesn't work but i'm just leaving it in for reference
	//UMovementComponent::MoveUpdatedComponent(const FVector& Delta, const FRotator& NewRotation, bool bSweep, FHitResult* OutHit, ETeleportType Teleport)
	//GetCharacterMovement()->MoveUpdatedComponent(MoveAdj, GetActorRotation(), true, &Hit);	
	//GetCharacterMovement()->MoveUpdatedComponent(-MoveAdj, GetActorRotation(), true, &Hit);			
}

// Sets default values
AHuman::AHuman()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;	
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("View Camera"));
	ViewCamera->SetupAttachment(CameraBoom);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

}

// Called when the game starts or when spawned
void AHuman::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(HumanMappingContext, 0);
		}
	}
	
}


// Called to bind functionality to input
void AHuman::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHuman::Move2D);	
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AHuman::Look);	
		//EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABird::Look);	
	}
}

// NOTE: Make sure to set up the "Orient Rotation To Movement" toggle under "Character Movement (Rotation Settings)"
// On the Character Movement component
void AHuman::Move2D(const FInputActionValue& Value)
{
	//if(ActionState != EActionSttate::EAS_Unoccupied) return; - implement later
	

	const FVector2D MovementVector = Value.Get<FVector2D>();
	UE_LOG(LogTemp, Display, TEXT("-------Move2D: %f, %f"), MovementVector.X, MovementVector.Y);

	const FRotator ControlRotation = GetControlRotation(); // rotation of the controller
	// only care about Yaw since we're moving parallel to the ground
	const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
	// now we need the directoin.  Tihs is where the rotation matricies come in
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); // X = forward vector
	AddMovementInput(ForwardDirection, MovementVector.Y);	

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);  // Y = right vector
	AddMovementInput(RightDirection, MovementVector.X);	

	// The way below will move the character but in the character's forward
	// and right, NOT in the way the camera is facing.  Above fixes that.
	/*FVector Forward = GetActorForwardVector();
	AddMovementInput(Forward, MovementVector.Y);	
	FVector Right = GetActorRightVector();
	AddMovementInput(Right, MovementVector.X);*/

}

void AHuman::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	//UE_LOG(LogTemp, Display, TEXT("-------Lood: %f, %f"), LookAxisVector.X, LookAxisVector.Y);

	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}


//------------------------- old way ------------------------------------------------------
#if false
void AEchoCharacter::MoveForward(float Value)
{
	//UE_LOG(LogTemp, Display, TEXT("-----------MoveForward: %f"), Value);
	if(Controller && (Value != 0.f))
	{
		// old way
		//FVector Forward = GetActorForwardVector();
		//AddMovementInput(Forward, Value);
		
		// Find out which way is forward.  
		// First we want to know the Controller's rotation.
		const FRotator ControlRotation = GetControlRotation(); // rotation of our controller
		// only care about Yaw since we're moving parallel to the ground
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		// now we need the direction.  This is where rotation matricies come in.
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); // X = forward vector
		AddMovementInput(Direction, Value);		
	}
}

void AEchoCharacter::MoveRight(float Value)
{
	if(Controller && (Value != 0.f))
	{
		// old way
		//FVector Right = GetActorRightVector();
		//AddMovementInput(Right, Value);

		// Find out which way is right
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y); // Y = right vector
		AddMovementInput(Direction, Value);		
	}
}

void AEchoCharacter::Turn(float Value)
{
	//UE_LOG(LogTemp, Display, TEXT("-------Turn: %f"), Value);
	AddControllerYawInput(Value);
}

void AEchoCharacter::LookUp(float Value)
{
	//UE_LOG(LogTemp, Display, TEXT("------LookUp: %f"), Value);
	AddControllerPitchInput(Value);
}
#endif
