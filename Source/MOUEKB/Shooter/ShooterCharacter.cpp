// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Gun.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
//
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket_MO"));
	Gun->SetOwner(this);
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(ShooterMappingContext, 0);
		}
	}
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		EnhancedInputComponent->BindAction(ShooterMoveAction, ETriggerEvent::Triggered, this, &AShooterCharacter::Move2D);	
		EnhancedInputComponent->BindAction(ShooterLookAction, ETriggerEvent::Triggered, this, &AShooterCharacter::Look);		
		EnhancedInputComponent->BindAction(ShooterJumpAction, ETriggerEvent::Triggered, this, &AShooterCharacter::Jump);	
		EnhancedInputComponent->BindAction(ShooterShootAction, ETriggerEvent::Triggered, this, &AShooterCharacter::Shoot);	
	}
}

void AShooterCharacter::Shoot()
{
	UE_LOG(LogTemp, Display, TEXT("------AShooterCharacter::Shoot()"));
	Gun->PullTrigger();
}

void AShooterCharacter::Move2D(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	//UE_LOG(LogTemp, Display, TEXT("-------Move2D(): %f, %f"), MovementVector.X, MovementVector.Y);

	const FRotator ControlRotation = GetControlRotation(); // rotation of the controller
	// only care about Yaw since we're moving parallel to the ground
	const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
	// now we need the directoin.  Tihs is where the rotation matricies come in
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); // X = forward vector
	AddMovementInput(ForwardDirection, MovementVector.Y);	

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);  // Y = right vector
	AddMovementInput(RightDirection, MovementVector.X);	
}

// NOTE: By default this won't work, so on the Spring Arm select "Use Pawn Control Rotation"
void AShooterCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	UE_LOG(LogTemp, Display, TEXT("-------Look(): %f, %f"), LookAxisVector.X, LookAxisVector.Y);

	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}

void AShooterCharacter::Jump()
{
	UE_LOG(LogTemp, Display, TEXT("--------Jump()"));
	ACharacter::Jump();
}

// --------------------- BELOW IS ALL BASIC STUFF FROM SECTION 06 SHOOTER ----------------------------------------
/*void AShooterCharacter_MO::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}
void AShooterCharacter_MO::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}
void AShooterCharacter_MO::LookUp(float AxisValue)
{	
	AddControllerPitchInput(AxisValue);
}
void AShooterCharacter_MO::LookRight(float AxisValue)
{	
	AddControllerYawInput(AxisValue);
}

void AShooterCharacter_MO::LookUpRate(float AxisValue)
{		
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}
void AShooterCharacter_MO::LookRightRate(float AxisValue)
{		
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}*/
