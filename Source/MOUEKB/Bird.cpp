// Fill out your copyright notice in the Description page of Project Settings.



#include "Bird.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// Called every frame
void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Sets default values
ABird::ABird()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(Capsule);

	BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bird Mesh"));
	BirdMesh->SetupAttachment(Capsule);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(Capsule);	

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("View Camera"));
	ViewCamera->SetupAttachment(CameraBoom);

	//Capsule->SetCapsuleHalfHeight(20.f); - can set up values in code
	//Capsule->SetCapsuleRadius(15.f);
	//CameraBoom->TargetArmLength = 400.f;
	//AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void ABird::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(BirdMappingContext, 0);
		}
	}
}

// Called to bind functionality to input
void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		// Move1D uses a Axis1D as it's value type (float).  Only showing it for demo purposes, the 2D way is the best way
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABird::Move1D);	
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABird::Look);	
	}

	// Old input
	/*PlayerInputComponent->BindAxis(TEXT("MoveForwardOld"), this, &ABird::MoveForwardOld);
	PlayerInputComponent->BindAxis(TEXT("TurnOld"), this, &ABird::TurnOld);
	PlayerInputComponent->BindAxis(TEXT("LookUpOld"), this, &ABird::LookUpOld);*/
}

/************************** Enhanced Input ********************************/
void ABird::Move1D(const FInputActionValue& Value)
{		
	float DirectionValue = Value.Get<float>();
	//UE_LOG(LogTemp, Display, TEXT("--------Move1D::bValue %f"), DirectionValue);		

	if(Controller && (DirectionValue != 0.f))
	{
		FVector Forward = GetActorForwardVector();
		AddMovementInput(Forward, DirectionValue);
	}
}

void ABird::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisValue = Value.Get<FVector2D>();
	//UE_LOG(LogTemp, Display, TEXT("----------Look: %f, %f"), LookAxisValue.X, LookAxisValue.Y);
	if(GetController())
	{		
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}

//******************** Old Input *************************************
void ABird::MoveForwardOld(float Value)
{
	//UE_LOG(LogTemp, Display, TEXT("-----------MoveForwardOld: %f"), Value);
	if(Controller && (Value != 0.f))
	{
		FVector Forward = GetActorForwardVector();
		AddMovementInput(Forward, Value);
	}
}

void ABird::TurnOld(float Value)
{
	//UE_LOG(LogTemp, Display, TEXT("-------TurnOld: %f"), Value);
	AddControllerYawInput(Value);
}

void ABird::LookUpOld(float Value)
{
	//UE_LOG(LogTemp, Display, TEXT("-------LookUpOld: %f"), Value);
	AddControllerPitchInput(Value);
}


