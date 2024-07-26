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

    // Basic Movement Speed
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Movement Parameters")
    float MovementSpeed = 400.0f;
    // Top Speed - to be used in the acceleration scenario
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Movement Parameters")
    float MaxUnitSpeed = 20.0f;
    // How fast we can accelerate
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Movement Parameters")
    float UnitAcceleration = 5.0f;
    // just added for testing purposes - so we can toggle sweep on and off
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Movement Parameters")
    bool bSweepRTS = false;

    /*
    ROTATION VARIABLEs
     */

    // Maximum Rotation Speed
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation Variables")
    float RotationSpeed = 2.0f;
    // Number of degrees acceptable to complete rotation
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation Variables")
    float RotationTolerance = 0.1f;
   
    /*
    LOGGING VARIABLEs
     */

    // how often to print to logg
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logging")
    float LoggingInterval = 2.0f;
    // toggle movement logging on/off
    UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Logging" )
    bool bLoggingEnabled = false;

private:
    // cached, casted pointer to the parent unit
    AUnitBase* OwningUnit; 

    // helpers for the "tick" event
    FVector GetNewVelocity(float DeltaTime);
    FRotator GetNewRotator(float DeltaTime);    
    FVector GetNewVelocityOnAccel(float DeltaTime);

    // push current rotator to the UI
    void PushRotator( FRotator& inRotator );

    // dev function
    float GetRemainingPathLength();
    float GetAcceptanceRadius();
    bool ShouldBrake( float DeltaTime );

    // used for logging
    float RunningLoggingTime { 0.0f };
    // used to calculate acceleration
    FVector CachedVelocity;
};
