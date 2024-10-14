// Copyright Jack Wedell 2024

#pragma once

#include "Core/Units/Base/RTSInteractableBase.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "RTSUnit.generated.h"

UCLASS()
class TANKRTS_API ARTSUnit : public ARTSInteractableBase {
    GENERATED_BODY()

public:
    // Sets default values for this pawn's properties
    ARTSUnit();

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
