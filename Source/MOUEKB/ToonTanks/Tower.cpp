// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"

void ATower::BeginPlay()
{
    Super::BeginPlay();

    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

    GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, 
        &ATower::CheckFireCondition, FireRate, true);
}

void ATower::CheckFireCondition()
{
    //UE_LOG(LogTemp, Display, TEXT("CheckFireCondition"));
    if(Tank == nullptr) { return; }

    float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());
    if(Distance <= FireRange && Tank->bAlive)
    {
        
        Fire();
    }
    else
    {
        //UE_LOG(LogTemp, Display, TEXT("Tank not in range."));
    }
}

void ATower::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(Tank)
    {                    
        // Find distance to the tank.  
        float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());
        if(Distance <= FireRange)
        {
            // If in range, rotate turret towards tank
            RotateTurret(Tank->GetActorLocation());
        }
    }        
}

void ATower::HandleDestruction()
{   
    Super::HandleDestruction();    
    UE_LOG(LogTemp, Display, TEXT("-------ATower::HandleDestruction()")); 
    Destroy();
    
}
