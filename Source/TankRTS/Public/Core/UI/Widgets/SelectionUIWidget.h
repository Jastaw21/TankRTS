// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
/**
 *
 */
#include "SelectionUIWidget.generated.h"

/**
 *
 */

UCLASS()
class TANKRTS_API USelectionUIWidget : public UUserWidget {
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite)
    FString TextToShow;
    UPROPERTY(BlueprintReadWrite)
    int NumUnitsSelected;

    void IncrementNumUnitsSelected();
    void ResetNumUnitsSelected();

    UFUNCTION(BlueprintCallable)
    FString GetJWUnitsAsText();

};
