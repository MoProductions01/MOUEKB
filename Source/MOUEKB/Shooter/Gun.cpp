// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene Component"));
	SetRootComponent(RootSceneComponent);

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun Mesh"));
	GunMesh->SetupAttachment(RootSceneComponent);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::PullTrigger()
{
	UE_LOG(LogTemp, Display, TEXT("-----AGun::PullTrigger()"));
	//DrawDebugCamera(GetWorld(), GetActorLocation(), GetActorRotation(), 90, 2, FColor::Red, true);
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, GunMesh, TEXT("MuzzleFlashSocket"));

	FVector Location;
	FRotator Rotation;	
	FHitResult Hit;
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(OwnerPawn == nullptr) return;
	AController* OwnerController = OwnerPawn->GetController(); if(OwnerController == nullptr) return;	
	OwnerController->GetPlayerViewPoint(Location, Rotation);	//virtual void GetPlayerViewPoint( FVector& Location, FRotator& Rotation ) const;
	// Rotation.Vector() gives you a vector pointing in the direction of this rotation.
	FVector End = Location + (Rotation.Vector() * MaxRange);	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	bool bSuccess = GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel2, Params);
	if(bSuccess)
	{
		FString name = Hit.GetActor()->GetActorNameOrLabel();
		UE_LOG(LogTemp, Display, TEXT("----AGun::PullTrigger() Hit: %s"), *name);		
		//DrawDebugCamera(GetWorld(), Location, Rotation, 90, 2, FColor::Red, true);
		DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, true);
		// shot direction shows us where the shot was coming from.
		FVector ShotDirection = -Rotation.Vector();			
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
		FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
		AActor* HitActor = Hit.GetActor();
		if(HitActor)
		{
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("-----------------No hit actor"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("----AGun::PullTrigger() Miss"));
	}
}

