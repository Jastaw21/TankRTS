// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "InteractableDetails.generated.h"

/**
 *
 */


UENUM()
enum InteractableType{
    Unit UMETA( DisplayName = "Unit" ),
    Building UMETA (DisplayName = "Building"),
    Resource UMETA (DisplayName = "Resource")
    
    };


USTRUCT( BlueprintType )
struct TANKRTS_API FInteractableDetails {
    GENERATED_BODY();

public:
    UPROPERTY( EditAnywhere, BlueprintReadWrite)
    FString GamePlayName;    

    UPROPERTY( EditDefaultsOnly, BlueprintReadWrite )
    TEnumAsByte<InteractableType> Interact_Type;

};
