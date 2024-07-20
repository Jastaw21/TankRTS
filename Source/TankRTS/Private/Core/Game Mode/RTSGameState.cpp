// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/Game Mode/RTSGameState.h"

ARTSGameState::ARTSGameState()
{
    UE_LOG(LogTemp, Display, TEXT("Game State Init"));
}

void ARTSGameState::BeginPlay()
{
    UE_LOG(LogTemp, Display, TEXT("Game State Init From Begin Play"));
}

TArray<ARTSInteractableBase*>& ARTSGameState::GetPlayerControlledInteractables()
    {
    return PlayerControlledInteractables;
    }

int ARTSGameState::GetNumPlayerControlledInteractables()
{
    return TempNumControlled;
}

void ARTSGameState::InsertPlayerControlledInteractable()
{
    TempNumControlled++;
}

void ARTSGameState::EmptyPlayerControlledInteractable()
    {
    PlayerControlledInteractables.Empty();
    }
