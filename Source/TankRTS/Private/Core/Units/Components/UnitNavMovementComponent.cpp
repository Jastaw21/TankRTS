// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/Units/Components/UnitNavMovementComponent.h"
#include "Core/Units/Base/UnitBase.h"

UUnitNavMovementComponent::UUnitNavMovementComponent()
{
    TObjectPtr<AActor> ActorPtr = GetOwner();
    if (ActorPtr) {

        OwningUnit = Cast<AUnitBase, AActor>(ActorPtr);
    }
}

void UUnitNavMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

    if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime)) {
        return;
    }

    else {

        FHitResult HitRes;
        SafeMoveUpdatedComponent(GetNewVelocity(DeltaTime), GetNewRotator(DeltaTime), bSweepRTS, HitRes);

        CachedVelocity = GetNewVelocity(DeltaTime);
        CachedRotator = GetNewRotator(DeltaTime);
    }
}

void UUnitNavMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
    Velocity = MoveVelocity;
}

void UUnitNavMovementComponent::RequestPathMove(const FVector& MoveInput)
{
    UE_LOG(LogTemp, Display, TEXT("RequestPathMove Speaking"));
}

FVector UUnitNavMovementComponent::GetNewVelocity(float DeltaTime)
{
    return Velocity.GetSafeNormal() * DeltaTime * MovementSpeed;
}

FRotator UUnitNavMovementComponent::GetNewRotator(float DeltaTime)
{
    FRotator DeltaRot = GetNewVelocity(DeltaTime).Rotation() - PawnOwner->GetActorRotation();
   
    float PitchVar = DeltaRot.Pitch;
    float YawVar = DeltaRot.Yaw;
    float RollVar = DeltaRot.Roll;

    float AbsDesiredRotationScale = FMath::Abs(PitchVar) + FMath::Abs(YawVar) + FMath::Abs(RollVar);

    float PitchScale = FMath::Abs(PitchVar) / AbsDesiredRotationScale;
    float YawScale = FMath::Abs(YawVar) / AbsDesiredRotationScale;
    float RollScale = FMath::Abs(RollVar) / AbsDesiredRotationScale;

    float MaxRotation = RotationSpeed * DeltaTime;

    float NewPitch = MaxRotation * PitchScale * FMath::Sign(PitchVar);
    float NewYaw = MaxRotation * YawScale * FMath::Sign(YawVar);
    float NewRoll = MaxRotation * RollScale * FMath::Sign(RollVar);

    return FRotator(NewPitch, NewYaw, NewRoll);
}
