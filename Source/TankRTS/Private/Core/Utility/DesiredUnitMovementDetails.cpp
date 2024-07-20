// Fill out your copyright notice in the Description page of Project Settings.

#include "TankRTS/Public/Core/Utility/DesiredUnitMovementDetails.h"

DesiredUnitMovementDetails::DesiredUnitMovementDetails()
{
}

DesiredUnitMovementDetails::~DesiredUnitMovementDetails()
{
}

void RectangleDetails::GetInPointsForm(FVector2D& inStart, FVector2D& inEnd)
{
    inStart.X = x1;
    inStart.Y = y1;

    inEnd.X = x2;
    inEnd.Y = y2;
}

void RectangleDetails::GetCorners(FVector2D& One, FVector2D& Two, FVector2D& Three, FVector2D& Four) // returns points starting clockwise from the top left
{
    // returns points starting clockwise from the top left

    One.X = x1,
    One.Y = y1;

    Two.X = x2;
    Two.Y = y1;

    Three.X = x2;
    Three.Y = y2;

    Four.X = x1;
    Four.Y = y2;
}

void RectangleDetails::SetEmpty()
{
    x1 = 0.0f;
    x2 = 0.0f;
    y1 = 0.0f;
    y2 = 0.0f;
}