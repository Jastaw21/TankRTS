// Fill out your copyright notice in the Description page of Project Settings.

#include "TankRTS/Public/Core/UI/HUD/TankRTSHud.h"
#include "Engine/Canvas.h"
#include "TankRTS/Public/Core/Controller/MyPlayerController.h"
#include "TankRTS/Public/Core/Units/Base/UnitBase.h"

void ATankRTSHud::DrawHUD()
{
    // all default stuff

    HitBoxMap.Reset();
    HitBoxHits.Reset();
    if (bShowOverlays && (PlayerOwner != nullptr)) {
        FVector ViewPoint;
        FRotator ViewRotation;
        PlayerOwner->GetPlayerViewPoint(ViewPoint, ViewRotation);
        DrawActorOverlays(ViewPoint, ViewRotation);
    }

    // all default stuff
    // Blueprint draw
    ReceiveDrawHUD(Canvas->SizeX, Canvas->SizeY);

    // test if the rectangle drawing has been initiated by the player controlled
    if (bShouldDrawRect) {
        DrawSelectionBox();
    }

    if (bShouldStopDrawingRectangle) {

        // which area was the rectangle last covering?
        FVector2D From;
        FVector2D To;
        LastDrawnRect.GetInPointsForm(From, To);

        // use the prebuilt fucntion to select
        GetActorsInSelectionRectangle(From, To, SelectedUnitArray, false, true);

        LastDrawnRect.SetEmpty();

        if (ControllerPtr != nullptr) {
            ControllerPtr->NotifyUnitsAreSelected();
        }

        bShouldStopDrawingRectangle = false;
    }
}

void ATankRTSHud::AskToDrawRect(float StartX, float StartY, float CurrentX, float CurrentY, AMyPlayerController* ControllerPtr_)
{
    bShouldDrawRect = true;
    LastDrawnRect.x1 = StartX;
    LastDrawnRect.y1 = StartY;
    LastDrawnRect.x2 = CurrentX;
    LastDrawnRect.y2 = CurrentY;

    // rebind our local ptr to the controller. Allows us to execute the delegate later, when we have selected units
    ControllerPtr = ControllerPtr_;
}

void ATankRTSHud::StopDrawingRect()
{
    bShouldStopDrawingRectangle = true;
    bShouldDrawRect = false;
}

TArray<AUnitBase*>* ATankRTSHud::GetSelectedUnits()
{
    return &SelectedUnitArray;
}

void ATankRTSHud::EmptyLocalUnitsCollection()
{
    SelectedUnitArray.Empty();
}

void ATankRTSHud::DrawSelectionBox()
{
    DrawRectangleHUD();
    DrawLines();
}
void ATankRTSHud::DrawRectangleHUD()
{
    DrawRect(SelectionColour, LastDrawnRect.x1, LastDrawnRect.y1, LastDrawnRect.w(), LastDrawnRect.h());
}
void ATankRTSHud::DrawLines()
{
    FVector2D One;
    FVector2D Two;
    FVector2D Three;
    FVector2D Four;
    LastDrawnRect.GetCorners(One, Two, Three, Four);

    Draw2DLine(One.X, One.Y, Two.X, Two.Y, LineColour);
    Draw2DLine(Two.X, Two.Y, Three.X, Three.Y, LineColour);
    Draw2DLine(Three.X, Three.Y, Four.X, Four.Y, LineColour);
    Draw2DLine(Four.X, Four.Y, One.X, One.Y, LineColour);
}
