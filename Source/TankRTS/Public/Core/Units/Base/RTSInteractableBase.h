// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/DecalComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "RTSInteractableBase.generated.h"

struct FInteractableDetails;

UCLASS()
class TANKRTS_API ARTSInteractableBase : public APawn {
    GENERATED_BODY()

    /*
    DEFAULT UE IMPLEMENTATIONS
    */

public:
    // Sets default values for this pawn's properties
    ARTSInteractableBase();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    /*
    TANK RTS Implementation
    */
public:
    // common data points for all items the player can interact with
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable Details");
    FInteractableDetails InteractableDetails;

    // access functions for the above
    FString* GetGameplayName();

    // all interactables will have a decal shown when selected
    UPROPERTY(EditAnywhere, Category = "Selection")
    UDecalComponent* SelectionDecal;

    // and a thumbnail to show on the UI
    UTexture2D* UIThumbnail;
};
