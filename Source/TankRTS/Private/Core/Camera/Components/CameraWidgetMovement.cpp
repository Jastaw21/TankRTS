// Fill out your copyright notice in the Description page of Project Settings.

#include "TankRTS/Public/Core/Camera/Components/CameraWidgetMovement.h"
#include "TankRTS/Public/Core/Camera/CameraWidget.h"
#include "GameFramework/SpringArmComponent.h"

void UCameraWidgetMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // conditions to ignore this function
    if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime) || !ControlledSpringArm) {
        return;
    }

    HandleLateral(DeltaTime);
    HandleZoom(DeltaTime);
    HandlePitch(DeltaTime);
    HandleYaw(DeltaTime);
}

void UCameraWidgetMovement::JAddLateralInput(FVector Input)
{
    DesiredAccel += Input;
}

void UCameraWidgetMovement::SetUpdatedSpringArm(USpringArmComponent* SpringArmComp)
{
    if (SpringArmComp) {
        ControlledSpringArm = SpringArmComp;
    }
}

float UCameraWidgetMovement::ConsumePitchInputVector()
{
    float CachedInputScalar = PitchInputScalar;
    PitchInputScalar = 0.0f;
    return CachedInputScalar;
}

float UCameraWidgetMovement::ConsumeYawInputVector()
{
    float CachedInputScalar = YawInputScalar;
    YawInputScalar = 0.0f;
    return CachedInputScalar;
}

float UCameraWidgetMovement::ConsumeZoomInput()
{
    float CachedInputScalar = ZoomInputScalar;
    ZoomInputScalar = 0.0f;
    return CachedInputScalar;
}

FVector UCameraWidgetMovement::ConsumeLateralInput()
{
    FVector CachedInput = DesiredAccel;
    DesiredAccel.Set(0.0f, 0.0f, 0.0f);
    return CachedInput;
}

void UCameraWidgetMovement::HandlePitch(float DeltaTime)
{
    // what is the current rotation
    FRotator CurrentRotation = ControlledSpringArm->GetRelativeRotation();
    // where do we want to go
    float DesiredPitch = CurrentRotation.Pitch + (PitchSpeed * DeltaTime * ConsumePitchInputVector());
    // make a rotator, with clamped pitch axis, and current yaw/roll axes
    FRotator DesiredRotation = FRotator(FMath::Clamp(DesiredPitch, MinimumPitch, MaximumPitch), CurrentRotation.Yaw, CurrentRotation.Roll);
    // sling that rotation back into the spring arm
    ControlledSpringArm->SetRelativeRotation(DesiredRotation);
}

void UCameraWidgetMovement::HandleYaw(float DeltaTime)
{
    // to Yaw, need to move the root component, so that it has the correct orientation for lateral movement
    UpdatedComponent->AddRelativeRotation(FRotator(0.0f, YawSpeed * DeltaTime * ConsumeYawInputVector(), 0));
}

void UCameraWidgetMovement::HandleZoom(float DeltaTime)
{
    float DesiredSpringArmLength = ControlledSpringArm->TargetArmLength += ConsumeZoomInput() * -ZoomSpeed * DeltaTime;
    ControlledSpringArm->TargetArmLength = FMath::Clamp(DesiredSpringArmLength, MinBoomLength, MaxBoomLength);
}

void UCameraWidgetMovement::HandleLateral(float DeltaTime)
{
    // control the lateral movement
    FHitResult hitRes;
    FVector desiredMovement = ConsumeLateralInput().GetClampedToMaxSize(1) * DeltaTime * MovementSpeed * FVector(1, 1, 0);
    SafeMoveUpdatedComponent(desiredMovement, UpdatedComponent->GetComponentRotation(), true, hitRes);
}
