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

        if (!bHasStartedLastLeg) {

            if (OnLastLeg()) {
                RotationOnStartingLastLeg = GetNewRotator(DeltaTime);
                bHasStartedLastLeg = true;
            }
        }

        FVector DesiredNewVelocity = GetNewVelocityOnAccel(DeltaTime);
        FRotator NewRotation;

        if (DesiredNewVelocity == FVector::ZeroVector) {
            NewRotation = RotationOnStartingLastLeg;
            ResetCachedRotationIfRequired(DeltaTime);
        }

        else {

            NewRotation = bIsOnLastLeg ? RotationOnStartingLastLeg : GetNewRotator(DeltaTime);
        }

        FHitResult HitRes;
        SafeMoveUpdatedComponent(DesiredNewVelocity, NewRotation, bSweepRTS, HitRes);
        CachedVelocity = DesiredNewVelocity;
    }
}

// helpers to extract calcs from the TickComponent function.

// returns true if we're inside the min distance required to deccelerate to a stop, based on the remaining AI path length
bool UUnitNavMovementComponent::ShouldBrake(float DeltaTime)
{
    FVector CurrentVelocity = CachedVelocity;
    float CurrentSpeed = CurrentVelocity.Size();
    // suvat equations
    float StoppingDistance = FMath::Pow(CurrentSpeed, 2) / (2 * FMath::Abs(UnitAcceleration));

    // include the "near side" of the acceptance radius
    return StoppingDistance >= (GetRemainingPathLength() - GetAcceptanceRadius());
}

bool UUnitNavMovementComponent::OnLastLeg()
{
    AAIController* Controller = Cast<AAIController, AController>(OwningUnit->GetController());
    if (IsValid(Controller)) {
        UPathFollowingComponent* PFComponent = Controller->GetPathFollowingComponent();
        if (PFComponent != nullptr && PFComponent->HasValidPath()) {
            const FNavPathSharedPtr PathToFollow = PFComponent->GetPath();

            int NextPointIndex = PFComponent->GetNextPathIndex();
            FNavPathPoint& LastPoint = PathToFollow->GetPathPoints().Last(0);
            return LastPoint.Location == PathToFollow->GetPathPoints()[NextPointIndex].Location;
        }
    }

    return false;
}

void UUnitNavMovementComponent::ResetCachedRotationIfRequired(float DeltaTime, float VelocityTolerance)
{
    if (FMath::Abs(GetNewVelocityOnAccel(DeltaTime).Size()) == 0.0f) {

        bIsOnLastLeg = false;
        bHasStartedLastLeg = false;
    }
}

// simple constant speed velocity - deprecated
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

    bool bShouldBrake = ShouldBrake(DeltaTime);

    /*
    In form of V = U + AT
    V = RawNewSpeed
    U = Cached Velocity
    A = UnitAcceleration
    T = Delta Time
    */
    float RawNewSpeed = CachedVelocity.Length() + (UnitAcceleration * DeltaTime * bShouldBrake ? -1.0f : 1.0f);

    // so far we just have raw accel - so clamp it to the MaxUnitSpeed.
    float ClampedNewSpeed = FMath::Clamp(RawNewSpeed, -MaxUnitSpeed, MaxUnitSpeed);

    // get the AI commanded velocity, turn it into a pure question of direction
    FVector NormalisedRequestedVelocity = Velocity.GetSafeNormal();

    // scale that raw direction vector, by the new sped
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

// code cleaning up functions - pulls out all the "IsValid" checks, and branching from the ShouldBrake() func
float UUnitNavMovementComponent::GetRemainingPathLength()
{
    AAIController* Controller = Cast<AAIController, AController>(OwningUnit->GetController());
    if (IsValid(Controller)) {

        UPathFollowingComponent* PFComponent = Controller->GetPathFollowingComponent();

        if (PFComponent != nullptr && PFComponent->HasValidPath()) {

            const FNavPathSharedPtr PathToFollow = PFComponent->GetPath();

            FVector RawActorPosition = OwningUnit->GetActorLocation();
            FNavPathPoint NextPoint = PathToFollow->GetPathPoints()[PFComponent->GetNextPathIndex()];
            FVector ZEqualisedActorPosition { RawActorPosition.X, RawActorPosition.Y, NextPoint.Location.Z };

            return PathToFollow->GetLengthFromPosition(
                ZEqualisedActorPosition, PFComponent->GetNextPathIndex());
        }
    }

    return -1.0f;
}
float UUnitNavMovementComponent::GetAcceptanceRadius()
{
    AAIController* Controller = Cast<AAIController, AController>(OwningUnit->GetController());
    if (IsValid(Controller)) {

        UPathFollowingComponent* PFComponent = Controller->GetPathFollowingComponent();

        if (PFComponent != nullptr && PFComponent->HasValidPath()) {

            return Controller->GetPathFollowingComponent()->GetAcceptanceRadius();
        }
    }

    return -1.0f;
}
