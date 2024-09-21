// Copyright Jack Wedell 2024

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "UnitAIStatus.generated.h"

/**
 *
 */

UENUM(BlueprintType)
enum UnitAIStatus {
    Rest UMETA(DisplayName = "Rest"),
    MovingTo UMETA(DisplayName = "Moving To Location"),
    Patrolling UMETA(DisplayName = "Patrolling")

};
