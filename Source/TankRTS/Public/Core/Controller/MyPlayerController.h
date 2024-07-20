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
    UPROPERTY( VisibleAnywhere, Category = "Unit Controller" )
    UUnitCommanderComponent* UnitCommanderComponent;
   
    // used by movement subcomponent
    FORCEINLINE ATankWidget* GetControlledCameraWidget() { return ControlledCameraWidget; }

    

    // selected unit UI components
    UPROPERTY(EditAnywhere)
    TSubclassOf<class USelectionUIWidget> PlayerUIClass;
    UPROPERTY()
    USelectionUIWidget* PlayerUI;

    // event to be triggered by the hud notifying units have been selected
    void NotifyUnitsAreSelected();

    FORCEINLINE USelectionUIWidget* GetSelectionUIWidget() { return PlayerUI; }

protected:
    ATankWidget* ControlledCameraWidget;
    TArray<AUnitBase*> SubscribedUnits; // Where we'll store pointers to the units under command
  
    ATankRTSHud* GetHUDCasted();

private:
    // trackers for rectangle drawing
    FVector2D MouseStart;
    FVector2D MouseEnd;
    bool IsMouseHeld = false;

   
    FORCEINLINE void MouseHoldON() { IsMouseHeld = true; } // toggles for the mouseheld bool
    FORCEINLINE void MouseHoldOFF() { IsMouseHeld = false; } // toggles for the mouseheld bool
    FORCEINLINE void SetMouseHold(bool in_IsMouseHeld) { IsMouseHeld = in_IsMouseHeld; } // toggles for the mouseheld bool
   

    // controls the selection rectangle
    void UpdateMouseXY(float Value);
    void TrackMousePosition();
    void StartRectangleDrawing();
    void EndRectangleDrawing();
    bool bIsMarqueeAllowed = false;

    FORCEINLINE void ToggleMarqueeOn() { bIsMarqueeAllowed = true; }
    FORCEINLINE void ToggleMarqueeOff() { bIsMarqueeAllowed = false; }


    FORCEINLINE void Speak() { UE_LOG( LogTemp, Display, TEXT( "Controller Speaking" ) ); }
};

