// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"

/**
Generated Sep
*/
#include "UnitNavMovementComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN( MovementLogs, Display, All );

class AUnitBase;

/**
AI Compatible Movement component to control RTS Units.
 */
UCLASS()
class TANKRTS_API UUnitNavMovementComponent : public UPawnMovementComponent {
    GENERATED_BODY()

public:
    UUnitNavMovementComponent();

    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    virtual void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;
    virtual void RequestPathMove(const FVector& MoveInput) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Variables")
    float MovementSpeed
        = 400.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Variables")
    float MaxUnitSpeed = 20.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Variables")
    float UnitAcceleration = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nav Variables")
    bool bSweepRTS = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Variables")
    float RotationSpeed = 2.0f;


    UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Movement Variables" )
    float RotationTolerance = 0.1f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logging")
    bool bLoggingEnabled = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logging")
    float LoggingInterval = 2.0f;

private:
    AUnitBase* OwningUnit;

    FVector CachedVelocity;
    FRotator CachedRotator;

    FVector GetNewVelocity(float DeltaTime);
    FRotator GetNewRotator(float DeltaTime);

    void PushRotator(FRotator& inRotator);

    float RunningLoggingTime { 0.0f };
};
