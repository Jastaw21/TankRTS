// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RTSGameState.generated.h"

class ARTSInteractableBase;

/**
 *
 */
UCLASS()
class TANKRTS_API ARTSGameState : public AGameStateBase {
    GENERATED_BODY()

public:
    ARTSGameState();
    virtual void BeginPlay() override;

    TArray<ARTSInteractableBase*>& GetPlayerControlledInteractables();
    int GetNumPlayerControlledInteractables();
    void InsertPlayerControlledInteractable();
    void EmptyPlayerControlledInteractable();

    int TempNumControlled = 0;

    void SetRotator(FRotator& InRotator);
    FString RotatorString;
    FString GetRotatorString();

protected:
private:
    TArray<ARTSInteractableBase*> PlayerControlledInteractables;
};
