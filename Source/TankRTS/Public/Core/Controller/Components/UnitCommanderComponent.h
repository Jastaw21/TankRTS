// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "UnitCommanderComponent.generated.h"

class AUnitBase;
class AMyPlayerController;
class UNiagaraSystem;
class USelectionUIWidget;
class ATankRTSHud;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TANKRTS_API UUnitCommanderComponent : public UActorComponent {
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UUnitCommanderComponent();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
    UNiagaraSystem* DestinationRing;

    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    void SetControlInputs(UInputComponent* Input);

private:
    TArray<AUnitBase*> SubscribedUnits;
    AMyPlayerController* Parent;

    ATankRTSHud* GetHUDCasted();

    // input actions handlers
    void SelectUnit(AUnitBase* UnitToSelect);
    void PollAreaUnderCursor();
    void GetUnitDestination();
    void DropAllSelectedUnits();
    void DrawHUDSelectionMarquee();
    void UpdateMousePositions(float Value);

    // input bindings
    FORCEINLINE void ToggleSelectionBoxBeingDrawn() { bIsSelectionBoxBeingDrawn = !(bIsSelectionBoxBeingDrawn); }
    FORCEINLINE void SetSelectionBoxBeingDrawnTrue() { bIsSelectionBoxBeingDrawn = true; }
    FORCEINLINE void SetSelectionBoxBeingDrawnFalse() { bIsSelectionBoxBeingDrawn = false; }

    void StartRectangleDrawing();
    void EndRectangleDrawing();

    FORCEINLINE void ToggleIsMouseHeld() { bIsMouseHeld = !(bIsMouseHeld); }
    FORCEINLINE void SetIsMouseHeldTrue() { bIsMouseHeld = true; }
    FORCEINLINE void SetIsMouseHeldFalse() { bIsMouseHeld = false; }

    bool bIsMouseHeld = false;
    bool bIsSelectionBoxBeingDrawn = false;

    FVector2D MouseStart;
    FVector2D MouseEnd;
};
