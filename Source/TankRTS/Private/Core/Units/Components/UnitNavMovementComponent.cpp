// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/Units/Components/UnitNavMovementComponent.h"

#include "Core/Game Mode/RTSGameState.h"
#include "Core/Units/Base/UnitBase.h"
#include "GameFramework/GameState.h"

DEFINE_LOG_CATEGORY(MovementLogs);

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
    RunningLoggingTime += DeltaTime;

    bool bShouldPrintToLog = (bLoggingEnabled && RunningLoggingTime >= LoggingInterval);

    FRotator DeltaRot = GetNewVelocity(DeltaTime).Rotation() - PawnOwner->GetActorRotation();

    if (bShouldPrintToLog) {
        UE_LOG(MovementLogs, Display, TEXT("Current Rotation: %s"), *PawnOwner->GetActorRotation().ToCompactString());
        UE_LOG(MovementLogs, Display, TEXT("Desired Rotation: %s"), *Velocity.Rotation().ToCompactString());
        UE_LOG(MovementLogs, Display, TEXT("Delta Rotation: %s"), *DeltaRot.ToCompactString());
    }

    float PitchVar = DeltaRot.Pitch;
    float YawVar = DeltaRot.Yaw;

    float AbsDesiredRotationScale = FMath::Abs(PitchVar) + FMath::Abs(YawVar);

    float PitchScale = FMath::Abs(PitchVar) / AbsDesiredRotationScale;
    float YawScale = FMath::Abs(YawVar) / AbsDesiredRotationScale;

    if (bShouldPrintToLog) {
        UE_LOG(MovementLogs, Display, TEXT("Total Scale %f"), (PitchScale + YawScale));
    }

    float MaxRotation = RotationSpeed * DeltaTime;

    float NewPitch = MaxRotation * PitchScale * FMath::Sign(PitchVar);
    float NewYaw = MaxRotation * YawScale * FMath::Sign(YawVar);

    FRotator ReturnRotator = FRotator(NewPitch, NewYaw, 0.0f);

    if (bShouldPrintToLog) {
        UE_LOG(MovementLogs, Display, TEXT("Output Rotation: %s"), *ReturnRotator.ToCompactString());
        RunningLoggingTime -= LoggingInterval;

        UE_LOG(MovementLogs, Display, TEXT("---------------------------"));
    }

    return ReturnRotator;
}

void UUnitNavMovementComponent::PushRotator(FRotator& inRotator)
{
    if (GetWorld()) {

        AGameStateBase* TempGame;
        TempGame = GetWorld()->GetGameState();
        if (TempGame) {
            ARTSGameState* GameState = Cast<ARTSGameState, AGameStateBase>(TempGame);
            GameState->SetRotator(inRotator);
        }
    }
}
