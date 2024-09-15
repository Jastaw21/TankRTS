// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
/**
 *
 */
#include "MyPlayerController.generated.h"

/**
 *
 */

// forward declarations
class ATankWidget;
class UCameraControllerComponent;
class UUnitCommanderComponent;
class ATankRTSHud;
class AUnitBase;
class USelectionUIWidget;
class UFormationManager;

// HUD Interface
DECLARE_DYNAMIC_DELEGATE(FNotifyThatUnitsAreSelected);

UCLASS()
class TANKRTS_API AMyPlayerController : public APlayerController {
    GENERATED_BODY()

public:
    AMyPlayerController();

    virtual void SetupInputComponent() override;

    // To add mapping context
    virtual void BeginPlay();

public:
    UPROPERTY(VisibleAnywhere, Category = "Camera")
    UCameraControllerComponent* CameraControllerComponent;

    UPROPERTY(VisibleAnywhere, Category = "Unit Controller")
    UUnitCommanderComponent* UnitCommanderComponent;

    UPROPERTY( VisibleAnywhere, Category = "Unit Controller" )
    UFormationManager* FormationManagerComponent;

    // selected unit UI components
    UPROPERTY(EditAnywhere)
    TSubclassOf<class USelectionUIWidget> PlayerUIClass;

    UPROPERTY()
    USelectionUIWidget* PlayerUI;

    // used by movement subcomponent
    FORCEINLINE ATankWidget* GetControlledCameraWidget() { return ControlledCameraWidget; }

    // event to be triggered by the hud notifying units have been selected
    void NotifyUnitsAreSelected();

    FORCEINLINE USelectionUIWidget* GetSelectionUIWidget() { return PlayerUI; }

    FORCEINLINE UUnitCommanderComponent* GetUnitCommander() { return UnitCommanderComponent; }

protected:
    ATankWidget* ControlledCameraWidget;
    ATankRTSHud* GetHUDCasted();

private:
    FORCEINLINE void Speak() { UE_LOG(LogTemp, Display, TEXT("Controller Speaking")); }


    
};
