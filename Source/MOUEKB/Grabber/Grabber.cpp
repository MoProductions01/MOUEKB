// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Display, TEXT("-------UGrabber::BeginPlay"));
	
	//GetPhysicsHandle();
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("-----------------------No PhysicsHandle Component"));
	}	
	return PhysicsHandle;
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FRotator MyRotation = GetComponentRotation();
	//FString RotationString = MyRotation.ToCompactString();
	//UE_LOG(LogTemp, Display, TEXT("-------Grabber rotation: %s"), *RotationString);
	//UE_LOG(LogTemp, Display, TEXT("------Current Time: %f"), GetWorld()->TimeSeconds);

	FVector Start = GetComponentLocation();	
	//FVector ForwardVector =  GetForwardVector();
	//FString VectorString = ForwardVector.ToCompactString();
	//UE_LOG(LogTemp, Display, TEXT("----------------Forward Vector: %s"), *VectorString);
	FVector End = Start + GetForwardVector() * MaxGrabDistance;
	//DrawDebugLine(GetWorld(), Start, End, FColor::Red);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	FHitResult HitResult;
	bool HasHit = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel1, Sphere);
	if(HasHit)
	{
		//AActor* HitActor = HitResult.GetActor();
		//UE_LOG(LogTemp, Display, TEXT("Hit Actor: %s"), *HitActor->GetActorNameOrLabel());
		DrawDebugSphere(GetWorld(), End, 10, 10, FColor::Blue, false);
	}
	else
	{
		//UE_LOG(LogTemp, Display, TEXT("----No Actor Hit"));
		DrawDebugSphere(GetWorld(), End, 10, 10, FColor::Red, false);
	}	


	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if(PhysicsHandle == nullptr) { UE_LOG(LogTemp, Error, TEXT("Null PhysicsHandle"));  return; }
	if(PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}	
}

void UGrabber::GrabberGrab()
{
	//UE_LOG(LogTemp, Display, TEXT("-------UGrabber::Grab()"));

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if(PhysicsHandle == nullptr) { UE_LOG(LogTemp, Error, TEXT("Null PhysicsHandle"));  return; }

	FHitResult HitResult;
	bool HasHit = GetGrabbableInReach(HitResult);
	if(HasHit)
	{
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		HitResult.GetActor()->Tags.Add("Grabbed"); // Tags is a TArray so we can just add to it
		// Gargoyle would only be grabbable if it was dropped originally, so make sure to
		// turn on all it's physics and use this to make sure it's moveable once grabbed.
		HitComponent->WakeAllRigidBodies(); 
		PhysicsHandle->GrabComponentAtLocationWithRotation(HitComponent, NAME_None, HitResult.ImpactPoint, 
										GetComponentRotation());

		AActor* HitActor = HitResult.GetActor();
		DrawDebugSphere(GetWorld(), HitResult.Location, 10, 10, FColor::Green, false, 5);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 10, FColor::Red, false, 5);
		UE_LOG(LogTemp, Display, TEXT("Grabbed Actor: %s"), *HitActor->GetActorNameOrLabel());					
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("---------Nothing Grabbed"));
	}	
}

void UGrabber::GrabberRelease()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if(PhysicsHandle == nullptr) { UE_LOG(LogTemp, Error, TEXT("Null PhysicsHandle"));  return; }

	if(PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("Released Grabbed Object"));
		AActor* GrabbedActor = PhysicsHandle->GetGrabbedComponent()->GetOwner();
		GrabbedActor->Tags.Remove("Grabbed");

		PhysicsHandle->GetGrabbedComponent()->WakeAllRigidBodies();
		PhysicsHandle->ReleaseComponent();
	}	
	else
	{
		UE_LOG(LogTemp, Display, TEXT("No Grabbed Object to release."));
	}
	
}

bool UGrabber::GetGrabbableInReach(FHitResult& OutHitResult) const
{
	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDistance;
	//DrawDebugLine(GetWorld(), Start, End, FColor::Red);	

	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);	
	return GetWorld()->SweepSingleByChannel(OutHitResult, Start, End,
														FQuat::Identity, ECC_GameTraceChannel1, Sphere);
		
}


