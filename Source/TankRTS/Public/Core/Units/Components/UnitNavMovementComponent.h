// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"

/**
Generated Sep
*/
#include "UnitNavMovementComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(MovementLogs, Display, All);

class AUnitBase;

/**
AI Compatible Movement component to control RTS Units.
 */
UCLASS()
class TANKRTS_API UUnitNavMovementComponent : public UPawnMovementComponent {
    GENERATED_BODY()

public:
    // ctor / called in the game loop
    UUnitNavMovementComponent();
    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // called by the AI controller
    virtual void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;
    virtual void RequestPathMove(const FVector& MoveInput) override;

    /*
    BASIC MOVEMENT VARS
    */
    // Top Speed - to be used in the acceleration scenario
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Movement Parameters")
    float MaxUnitSpeed = 12.0f;
    // How fast we can accelerate
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Movement Parameters")
    float UnitAcceleration = 0.2f;
    // just added for testing purposes - so we can toggle sweep on and off
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Movement Parameters")
    bool bSweepRTS = false;

    /*
    ROTATION VARIABLEs
     */
    // Maximum Rotation Speed
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation Variables")
    float RotationSpeed = 360.0f;
    // Number of degrees acceptable to complete rotation
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation Variables")
    float RotationTolerance = 0.1f;

    /*
    LOGGING VARIABLEs
     */
    // how often to print to logg
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logging")
    float GroundScanInterval = 0.1f;
    // toggle movement logging on/off
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logging")
    bool bLoggingEnabled = false;

   

private:
    /*
    NEXT MOVE CALCULATORS
    */
    FRotator GetVelocityRotator(float DeltaTime);
    FVector GetNewVelocity(float DeltaTime);


    /*
    INTERFACES TO AI CONTROLLER
    */
    float GetRemainingPathLength();
    float GetAcceptanceRadius();
    FRotator GetAICommandedRotation();
    float GetCommandedAndActualYawVariance(const FRotator& Commanded, const FRotator& Actual );
    void DrawPathLines();


    /*
    POSITION CHECKERS
    */
    bool ShouldBrake(float DeltaTime); // returns true if we're in the decelleration zone
    bool ActorIsOnLastLeg(); // check if we're on the last leg. Called every frame if not already set to true
    void HandleGroundInteraction(float DeltaTime);

    // cached, casted pointer to the parent unit
    AUnitBase* OwningUnit;

    float GroundScanRunningTime { 0.0f };
    // used to calculate acceleration
    FVector CachedVelocity;

    // data to prevent rotation back towards (0,0,0) when reaching destination
    FRotator RotationOnStartingLastLeg;

    // toggled to true when entering the last leg - set back to false when velocity drops to within a tolerance (TBC) of zero
    // idea being - when we first see this is true, archive the rotation - and don't change that until we restart movement - prevents returning back to (0,0,0)
    bool bIsOnLastLeg = false;
    bool bHasStartedLastLeg = false;
    bool isUnitMoving = false;


    FVector MovementInput;
};
