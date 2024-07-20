// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TankRTS/Public/Core/Utility/DesiredUnitMovementDetails.h"
#include "GameFramework/HUD.h"


//
//
#include "TankRTSHud.generated.h"

class AUnitBase;
class AMyPlayerController;



UCLASS()
class TANKRTS_API ATankRTSHud : public AHUD{

    GENERATED_BODY()


public:
    virtual void DrawHUD() override;

    

    /**
    Editable properties to update the colours
    **/
    UPROPERTY(EditAnywhere, BlueprintReadWrite) // colours of the main selection box
    FColor SelectionColour { 0, 0, 255, 25 };
    UPROPERTY( EditAnywhere, BlueprintReadWrite ) // colour for the outline of the selection box
    FColor LineColour{0, 0, 255, 100};


    /**
    Player controller interface.
    Probably better to extract this to operate via delegates or interfaces later, but works for now    
    **/
    // to be used by the player controller, to start drawing the rectangle
    void AskToDrawRect(float in_X, float in_Y, float in_CurrentXPos, float in_CurrentYPos, AMyPlayerController* ControllerPtr_);
    // to be used by the player controller, to cancel drawing the rectangle
    void StopDrawingRect();
    
    void EmptyLocalUnitsCollection();

    TArray<AUnitBase*>* GetSelectedUnits( );


private:
    // Useful to have a cache of where the last rectangle was drawn, so it can be passed to the selection fx
    RectangleDetails LastDrawnRect; 

    // flag for the controller to "release" the rectangle drawing ability
    bool bShouldDrawRect = false;

    // flag, toggled when the controller indicates it's done with the rectangle    
    bool bShouldStopDrawingRectangle = false;

    // where we'll store the units we've collected
    TArray<AUnitBase*> SelectedUnitArray;
    
    /**
    Internal drawing functions
    **/
    void DrawSelectionBox(); // dispatcher function, calls the next two.
    void DrawRectangleHUD(); // call to draw the box only
    void DrawLines(); // draws the border around the selection rectangle.

    AMyPlayerController* ControllerPtr;
    
};
