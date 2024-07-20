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
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    void SetControlInputs(UInputComponent* Input);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
    UNiagaraSystem* DestinationRing;

private:
    TArray<AUnitBase*> SubscribedUnits;
    AMyPlayerController* Parent;
    ATankRTSHud* GetHUDCasted();


    void SelectUnit(AUnitBase* UnitToSelect);

    void PollAreaUnderCursor();
    void GetUnitDestination();
    void DropAllSelectedUnits();

};
