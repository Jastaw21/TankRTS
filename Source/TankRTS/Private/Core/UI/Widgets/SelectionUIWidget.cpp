// Fill out your copyright notice in the Description page of Project Settings.

#include "TankRTS/Public/Core/UI/Widgets/SelectionUIWidget.h"

void USelectionUIWidget::ResetNumUnitsSelected()
{
    NumUnitsSelected = 0;
    TextToShow = FString::FromInt(NumUnitsSelected);
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
    }
    else {
        Value = FString::FromInt(NumUnitsSelected);
    }

    return Value;
}
