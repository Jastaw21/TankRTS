// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/Units/Components/UnitNavMovementComponent.h"
#include "Core/Game Mode/RTSGameState.h"
#include "Core/Units/Base/UnitBase.h"
#include "GameFramework/GameState.h"
#include "Navigation/PathFollowingComponent.h"

#include "AIController.h"

DEFINE_LOG_CATEGORY(MovementLogs);

// ctor
UUnitNavMovementComponent::UUnitNavMovementComponent()
{
    TObjectPtr<AActor> ActorPtr = GetOwner();
    if (ActorPtr) {

        OwningUnit = Cast<AUnitBase, AActor>(ActorPtr);
    }
}
// called by the AI Controller
void UUnitNavMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
    Velocity = MoveVelocity;
}
void UUnitNavMovementComponent::RequestPathMove(const FVector& MoveInput)
{
    UE_LOG(LogTemp, Display, TEXT("RequestPathMove Speaking"));
}

// called in the main game loop
void UUnitNavMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

    if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime)) {
        return;
    }

    else {
        FHitResult HitRes;
        SafeMoveUpdatedComponent(GetNewVelocityOnAccel(DeltaTime), GetNewRotator(DeltaTime), bSweepRTS, HitRes);
        CachedVelocity = GetNewVelocityOnAccel(DeltaTime);

        float PathLen = GetRemainingPathLength();

        if (PathLen > 0.0f) {
            UE_LOG(LogTemp, Display, TEXT("PATH LENGTH %f:"), PathLen);
        }
    }
}

// helpers to extract calcs from the TickComponent function.
FVector UUnitNavMovementComponent::GetNewVelocity(float DeltaTime)
{
    return Velocity.GetSafeNormal() * DeltaTime * MovementSpeed;
}
FRotator UUnitNavMovementComponent::GetNewRotator(float DeltaTime)
{
    // control loggin behaviour
    RunningLoggingTime += DeltaTime;
    bool bShouldPrintToLog = (bLoggingEnabled && RunningLoggingTime >= LoggingInterval);

    // what is the change in desired rotation
    FRotator DeltaRot = GetNewVelocity(DeltaTime).Rotation() - PawnOwner->GetActorRotation();

    // if we're within the rotation tolerance - early out
    if (DeltaRot.IsNearlyZero(RotationTolerance)) {
        return PawnOwner->GetActorRotation();
    }

    // logging step 1
    if (bShouldPrintToLog) {
        UE_LOG(MovementLogs, Display, TEXT("Current Rotation: %s"), *PawnOwner->GetActorRotation().ToCompactString());
        UE_LOG(MovementLogs, Display, TEXT("Desired Rotation: %s"), *Velocity.Rotation().ToCompactString());
        UE_LOG(MovementLogs, Display, TEXT("Delta Rotation: %s"), *DeltaRot.ToCompactString());
    }

    // how big is the required rotation in each axis
    float PitchVar = DeltaRot.Pitch;
    float YawVar = DeltaRot.Yaw;

    // and then what percentage of the total magnitude does each axis provide
    float AbsDesiredRotationScale = FMath::Abs(PitchVar) + FMath::Abs(YawVar);
    float PitchScale = FMath::Abs(PitchVar) / AbsDesiredRotationScale;
    float YawScale = FMath::Abs(YawVar) / AbsDesiredRotationScale;

    // logging step 2
    if (bShouldPrintToLog) {
        UE_LOG(MovementLogs, Display, TEXT("Total Scale %f"), (PitchScale + YawScale));
    }

    // how long should the magnitude be?
    float MaxRotation = RotationSpeed * DeltaTime;

    // return each axis to the scaled place
    float NewPitch = MaxRotation * PitchScale * FMath::Sign(PitchVar);
    float NewYaw = MaxRotation * YawScale * FMath::Sign(YawVar);

    // add the new calculated incremental rotation to the existing one
    FRotator ReturnRotator = PawnOwner->GetActorRotation() + FRotator(NewPitch, NewYaw, 0.0f);

    // final loggin step
    if (bShouldPrintToLog) {
        UE_LOG(MovementLogs, Display, TEXT("Output Rotation: %s"), *ReturnRotator.ToCompactString());
        RunningLoggingTime -= LoggingInterval;

        UE_LOG(MovementLogs, Display, TEXT("---------------------------"));
    }

    return ReturnRotator;
}
FVector UUnitNavMovementComponent::GetNewVelocityOnAccel(float DeltaTime)
{

    // v = u+ at
    float u = CachedVelocity.Length(); // original speed

    float RawNewSpeed = u + (UnitAcceleration * DeltaTime);
    float ClampedNewSpeed = FMath::Clamp(RawNewSpeed, -MaxUnitSpeed, MaxUnitSpeed);

    FVector RequestedVelocity = Velocity;
    FVector NormalisedRequestedVelocity = RequestedVelocity.GetSafeNormal();

    return ClampedNewSpeed * NormalisedRequestedVelocity;
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

float UUnitNavMovementComponent::GetRemainingPathLength()
{
    AAIController* AIController = Cast<AAIController, AController>(OwningUnit->GetController());
    if (IsValid(AIController)) {

        UPathFollowingComponent* PathFollowingaaComp = AIController->GetPathFollowingComponent();

        if (PathFollowingaaComp != nullptr && PathFollowingaaComp->HasValidPath()) {
            const FNavPathSharedPtr PathToFollow = PathFollowingaaComp->GetPath();
            //UE_LOG(LogTemp, Display, TEXT("Current Index: %d"), PathFollowingaaComp->GetCurrentPathIndex());
            FVector RawActorPosition = OwningUnit->GetActorLocation();
            FNavPathPoint NextPoint = PathToFollow->GetPathPoints()[PathFollowingaaComp->GetNextPathIndex()];
            FVector ZEqualisedActorPosition{RawActorPosition.X, RawActorPosition.Y, NextPoint.Location.Z};
            return PathToFollow->GetLengthFromPosition( ZEqualisedActorPosition, PathFollowingaaComp->GetNextPathIndex());
        }
    }

    return -1.0f;
}