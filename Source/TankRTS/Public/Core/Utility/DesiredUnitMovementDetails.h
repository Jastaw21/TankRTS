// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */

class TANKRTS_API DesiredUnitMovementDetails {
public:
    DesiredUnitMovementDetails();
    ~DesiredUnitMovementDetails();

    FVector DesiredLocation;
    bool HasDesiredLocation = false;
};

struct TANKRTS_API RectangleDetails {
    float x1;
    float y1;
    float x2;
    float y2;

    void GetInPointsForm(FVector2D& inStart, FVector2D& inEnd);
    void GetCorners(FVector2D& One, FVector2D& Two, FVector2D& Three, FVector2D& Four);
    void SetEmpty();
    FORCEINLINE float w() { return x2 - x1; }
    FORCEINLINE float h() { return y2 - y1; }
};