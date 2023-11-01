// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"

ATank::ATank()
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);    
}

void ATank::BeginPlay()
{
	Super::BeginPlay();

    TankPlayerController = Cast<APlayerController>(GetController());

	//if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    if(TankPlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(TankPlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(TankMappingContext, 0);
		}
	}	
}



void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
        EnhancedInputComponent->BindAction(Move1DAction, ETriggerEvent::Triggered, this, &ATank::Move1D);	
        EnhancedInputComponent->BindAction(Turn1DAction, ETriggerEvent::Triggered, this, &ATank::Turn1D);	
        EnhancedInputComponent->BindAction(TankFireAction, ETriggerEvent::Triggered, this, &ATank::Fire);	
	//	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHuman::Move2D);	
	//	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AHuman::Look);	
		//EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABird::Look);	
	}
}

void ATank::Move1D(const FInputActionValue& Value)
{      
    float MoveValue = Value.Get<float>();  
    //UE_LOG(LogTemp, Display, TEXT("---------Move1D Value: %f"), MoveValue);    
    // Note: Tried with the ForwardVector 'n AddMovementInput stuff but didn't work. 
    // Might be because this is a Pawn and not a Character.
    FVector DeltaLocation = FVector::ZeroVector;
    float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	DeltaLocation.X = MoveValue * Speed * DeltaTime;
	AddActorLocalOffset(DeltaLocation, true);    
}

void ATank::Turn1D(const FInputActionValue& Value)
{
    float TurnValue = Value.Get<float>();  
   // UE_LOG(LogTemp, Display, TEXT("---------Turn1D Value: %f"), TurnValue);   
    FRotator DeltaRotation = FRotator::ZeroRotator;
    float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
    DeltaRotation.Yaw = TurnValue * TurnRate * DeltaTime;
    AddActorLocalRotation(DeltaRotation);
	// Yaw set to Value * DeltaTime * TurnRate	
}

void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(TankPlayerController)
    {
        FHitResult HitResult;
        TankPlayerController->GetHitResultUnderCursor(
            ECollisionChannel::ECC_Visibility, false, HitResult);

        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 25.f, 12,
                    FColor::Red, false, -1.f);

        RotateTurret(HitResult.ImpactPoint);                
    }
}

void ATank::Fire()
{
    Super::Fire();
	//UE_LOG(LogTemp, Display, TEXT("----------------------ATank::Fire!"));
}