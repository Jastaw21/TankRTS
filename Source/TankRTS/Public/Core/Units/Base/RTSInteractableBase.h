// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/DecalComponent.h"
#include "Core/Interactables/InteractableDetails.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "RTSInteractableBase.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(RTSInteractables, Display, All);

class UCapsuleComponent;

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

    // all interactables will have a decal shown when selected
    UPROPERTY(EditAnywhere, Category = "Selection")
    UDecalComponent* SelectionDecal;

    UPROPERTY(EditAnywhere, Category = "Selection")
    // and a thumbnail to show on the UI
    UTexture2D* UIThumbnail;

    UPROPERTY(EditAnywhere, Category = "Mesh")
    UStaticMeshComponent* Mesh; // the main mesh

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UCapsuleComponent> CapsuleComponent;

    // access functions for the above
    FString* GetGameplayName();
    InteractableType GetInteractableType();

    virtual void Speak();
    virtual ARTSInteractableBase* OnClickedRTS();
    virtual ARTSInteractableBase* OnDroppedRTS();
};
