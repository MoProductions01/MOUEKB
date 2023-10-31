// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

// Called when the game starts
void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Display, TEXT("-------UTriggerComponent::BeginPlay"));	
}

// Sets default values for this component's properties
UTriggerComponent::UTriggerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

    UE_LOG(LogTemp, Display, TEXT("----------------------Constructing UTriggerComponent"));
	// ...
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    //UE_LOG(LogTemp, Display, TEXT("-----------------------------------------UTriggerComponent::TickComponent"));

    /*TArray<AActor*> Actors;
    GetOverlappingActors(Actors);
    // Ranged Based For Loop
    for(AActor* Actor : Actors) // basically goes over every Actor* in TArrays (works with all sorts of collection types)
    {       
        FString ActorName = Actor->GetActorNameOrLabel();
        if(Actor->ActorHasTag(AcceptableActorTag))
        {            
            UE_LOG(LogTemp, Display, TEXT("----------------------Has correct tag so Overlapping : %s"), *ActorName);           
        }    
        else
        {
            UE_LOG(LogTemp, Display, TEXT("----------------Incorrect tag: %s"), *ActorName);
        }    
    }*/


    /// array stuff below.
   /* if(Actors.Num() > 0)
    {
        FString ActorName = Actors[0]->GetActorNameOrLabel();
        UE_LOG(LogTemp, Display, TEXT("----------------------Overlapping: %s"), *ActorName);        
    }*/
    // Ranged Based For Loop
    /*for(AActor* Actor : Actors) // basically goes over every Actor* in TArrays (works with all sorts of collection types)
    {       
        FString ActorName = Actor->GetActorNameOrLabel();
        UE_LOG(LogTemp, Display, TEXT("---------99-------------Overlapping : %s"), *ActorName);           
    }*/

    AActor* Actor = GetAcceptableActor();
    if(Actor != nullptr)
    {
        //UE_LOG(LogTemp, Display, TEXT("------UTriggerComponent::Tic have acceptable actor: %s"), *Actor->GetActorNameOrLabel());
        UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
        if(Component != nullptr)
        {
            Component->SetSimulatePhysics(false);
        }
        Actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
        MovingWall->SetShouldMove(true);
        
    }
    else
    {
       // UE_LOG(LogTemp, Display, TEXT("------UTriggerComponent::Tick no acceptable actor found"));
        MovingWall->SetShouldMove(false);
    }
}

AActor* UTriggerComponent::GetAcceptableActor() const
{
    TArray<AActor*> Actors;
    GetOverlappingActors(Actors);
    for(AActor* Actor : Actors) // basically goes over every Actor* in TArrays (works with all sorts of collection types)
    {
        if( Actor->ActorHasTag(AcceptableActorTag) && !Actor->ActorHasTag("Grabbed"))
        {
           // FString ActorName = Actor->GetActorNameOrLabel();
           // UE_LOG(LogTemp, Display, TEXT("---------99-------------Overlapping : %s"), *ActorName);   
          // UE_LOG(LogTemp, Display, TEXT("--------------------------------Unlocking"));
           return Actor;
        }           
    }

    return nullptr;
}

// call it NewMovingWall instead of MovingWall because of Shadow Casting.  Causes errors.
void UTriggerComponent::SetMovingWall(UMovingWall* NewMovingWall)
{
	MovingWall = NewMovingWall;
}
