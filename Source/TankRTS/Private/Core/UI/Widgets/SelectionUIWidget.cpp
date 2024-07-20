// Fill out your copyright notice in the Description page of Project Settings.

#include "TankRTS/Public/Core/UI/Widgets/SelectionUIWidget.h"

#include "Core/Game Mode/RTSGameState.h"
#include "GameFramework/GameStateBase.h"

void USelectionUIWidget::ResetNumUnitsSelected()
{
    NumUnitsSelected = 0;
    TextToShow = "";
}

void USelectionUIWidget::IncrementNumUnitsSelected()
{
    NumUnitsSelected += 1;
    TextToShow = FString::FromInt(NumUnitsSelected);
}

FString USelectionUIWidget::GetJWUnitsAsText()
{
    FString Value;
    if (NumUnitsSelected == 0) {
        Value = "";
    } else {
        Value = FString::FromInt(NumUnitsSelected);
    }

    return Value;
}

int USelectionUIWidget::GetNumberUnits()
{

    if (GetWorld()) {
        AGameStateBase* TempGameState = GetWorld()->GetGameState();

        if (TempGameState) {
            ARTSGameState* RTSState = Cast<ARTSGameState, AGameStateBase>(TempGameState);
            return RTSState->GetNumPlayerControlledInteractables();
        }
    }

    return -1;
}
