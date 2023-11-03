// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksPlayerController.h"

void AToonTanksPlayerController::SetPlayerEnabledState(bool bPlayerEnabled)
{
    UE_LOG(LogTemp, Display, TEXT("---------------SetPlayerEnabledState: %d"), bPlayerEnabled);
    if(bPlayerEnabled)
    {
        GetPawn()->EnableInput(this);
    }
    else
    {
        GetPawn()->DisableInput(this);
    }
    // bShowMouseCursor is in PlayerController.h
    bShowMouseCursor = bPlayerEnabled;
}