// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Tower.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanksPlayerController.h"

void AToonTanksGameMode::BeginPlay()
{
    Super::BeginPlay();

    HandleGameStart();
}

void AToonTanksGameMode::ActorDied(AActor* DeadActor)
{
    UE_LOG(LogTemp, Display, TEXT("---------AToonTanksGameMode::ActorDied()"));

    if(DeadActor == Tank)
    {
        Tank->HandleDestruction();
       // if(Tank->GetTankPlayerController())
        if(ToonTanksPlayerController)
        {
            //Tank->DisableInput(Tank->GetTankPlayerController());
            //Tank->GetTankPlayerController()->bShowMouseCursor = false;
            ToonTanksPlayerController->SetPlayerEnabledState(false);
        }
        UE_LOG(LogTemp, Display, TEXT("-------- call GameOverWidgetStart(false)"));
        GameOverWidgetStart(false);
    }    
    else if(ATower* DestroyedTower = Cast<ATower>(DeadActor))
    {
        DestroyedTower->HandleDestruction();
        TargetTowers--;
        UE_LOG(LogTemp, Display, TEXT("--------Destroyed tower.  Num left: %d"), TargetTowers);
        if(TargetTowers == 0)
        {
            GameOverWidgetStart(true);
        }
    }
}

void AToonTanksGameMode::HandleGameStart()
{
    TargetTowers = GetTargetTowerCount();
    UE_LOG(LogTemp, Display, TEXT("-----We have %d towers in the game"), TargetTowers);
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));    
    ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
    
    StartGameWidgetStart();
    
    if(ToonTanksPlayerController)
    {
        ToonTanksPlayerController->SetPlayerEnabledState(false);

        // need to do things this way because our callback has a parameter
        FTimerHandle PlayerEnableTimerHandle;
        FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(ToonTanksPlayerController, 
            &AToonTanksPlayerController::SetPlayerEnabledState, true);
        GetWorldTimerManager().SetTimer(PlayerEnableTimerHandle, PlayerEnableTimerDelegate, StartDelay, false);     
    }
}

int32 AToonTanksGameMode::GetTargetTowerCount()
{
    TArray<AActor*> Towers;
    UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), Towers);
    return Towers.Num();    
} 