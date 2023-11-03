// Fill out your copyright notice in the Description page of Project Settings.


#include "PushingCube.h"

// Sets default values
APushingCube::APushingCube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>("Pushing Cube Mesh");
	CubeMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APushingCube::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();
	FString Name = GetName();
	UE_LOG(LogTemp, Display, TEXT("-------- APushingCube::BeginPlay(): %s"), *Name);
}

// Called every frame
void APushingCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CurrentLocation = GetActorLocation();
	CurrentLocation += PlatformVelocity * DeltaTime;
	SetActorLocation(CurrentLocation);
	DistanceMoved = FVector::Dist(StartLocation, CurrentLocation);
	if(DistanceMoved >= MoveDistance)
	{
		//float OverShoot = DistanceMoved - MoveDistance;
		PlatformVelocity = -PlatformVelocity;
		StartLocation = CurrentLocation;
	}

}

