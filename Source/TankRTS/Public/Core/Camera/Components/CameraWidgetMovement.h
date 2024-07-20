// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "UObject/ObjectMacros.h"
// xx
// **
// xx
#include "CameraWidgetMovement.generated.h"

/**
 *
 */

class USpringArmComponent;

UCLASS()
class TANKRTS_API UCameraWidgetMovement : public UPawnMovementComponent {
    GENERATED_BODY()

public:
    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // functions to add to the cached input variables
    FORCEINLINE void JAddYawInput(float Value) { YawInputScalar += Value; }
    FORCEINLINE void JAddPitchInput(float Value) { PitchInputScalar += Value; }
    FORCEINLINE void JAddZoomInput(float Value) { ZoomInputScalar += Value; }

    FORCEINLINE void JAddLateralInput( FVector Input );

    void SetUpdatedSpringArm(USpringArmComponent* SpringArmComp);

protected:
    // Movement speed vars
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niche Cats")
    float MovementSpeed = 900.0f;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boom Movement Parameters");
    float YawSpeed = 360.0f;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boom Movement Parameters");
    float PitchSpeed = 360.0f;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Zoom Parameters");
    float ZoomSpeed = 1150.0f;

    // Movement bounds
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Zoom Parameters");
    float MinBoomLength = 300.0f;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Zoom Parameters");
    float MaxBoomLength = 1200.0f;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement Bounds");
    float MinimumPitch = -150.0f;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement Bounds");
    float MaximumPitch = -10.0f;

private:
    // Pitch Input/Consumption
    float PitchInputScalar = 0.0f;
    float ConsumePitchInputVector();

    // Yaw Input/Consumption
    float YawInputScalar = 0.0f;
    float ConsumeYawInputVector();

    // Zoom Input/Consumption
    float ZoomInputScalar = 0.0f;
    float ConsumeZoomInput();

    // Lateral Input/Consumption
    FVector DesiredAccel;
    FVector ConsumeLateralInput();

    // internal handlers of the movement components
    void HandlePitch(float DeltaTime);
    void HandleYaw(float DeltaTime);
    void HandleZoom(float DeltaTime);
    void HandleLateral(float DeltaTime);

    // somewhere to cache a ptr to the spring arm, to make it more efficient
    USpringArmComponent* ControlledSpringArm;
};
