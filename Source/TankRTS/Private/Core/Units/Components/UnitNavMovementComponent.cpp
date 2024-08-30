// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/Units/Components/UnitNavMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Core/Game Mode/RTSGameState.h"
#include "Core/Units/Base/UnitBase.h"
#include "Core/Utility/JWMath.h"
#include "GameFramework/GameState.h"
#include "Kismet/KismetMathLibrary.h"
#include "Navigation/PathFollowingComponent.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"

#include "AIController.h"

DEFINE_LOG_CATEGORY(MovementLogs);

/*
DEFAULT FUNCTIONS
*/

// ctor
UUnitNavMovementComponent::UUnitNavMovementComponent()
{

    DistanceToScanDown = 200.0f;

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

void UUnitNavMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

    if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime)) {
        return;
    }

    else {

        FVector NewDirection = GetNewVelocity(DeltaTime);
        FRotator NewRotation = GetVelocityRotator(DeltaTime);

        if (SubsequentCollisons <= MaxCollisionsBeforeCheating) {

            FHitResult Hit;

            if (!SafeMoveUpdatedComponent(NewDirection, NewRotation, true, Hit))

            {
                SubsequentCollisons += 1;

                SlideAlongSurface(NewDirection, 1.0f - Hit.Time, Hit.Normal, Hit);
            }
        }

        else {

            MoveUpdatedComponent(NewDirection, NewRotation, false);
            SubsequentCollisons = 0;
        }

        CachedVelocity = NewDirection;
        CachedVelocity = NewDirection;
        // HandleGroundInteraction(DeltaTime);
    }
}

// NEXT MOVE CALCULATORS
FRotator UUnitNavMovementComponent::GetVelocityRotator(float DeltaTime)
{

    FRotator RawVelocRotation = Velocity.ToOrientationRotator();

    FRotator DeltaRot = RawVelocRotation - PawnOwner->GetActorRotation();

    // if we're within the rotation tolerance - early out
    if (DeltaRot.IsNearlyZero(RotationTolerance)) {

        return PawnOwner->GetActorRotation();
    }

    else {

        JWMath::ScaleVectorToSpeed(DeltaRot, RotationSpeed * DeltaTime);
        // add the new calculated incremental rotation to the existing one
        return PawnOwner->GetActorRotation() + DeltaRot;
    }
}
FVector UUnitNavMovementComponent::GetNewVelocity(float DeltaTime)
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
    return ClampedNewSpeed * NormalisedRequestedVelocity * FVector(1, 1, 0);
}
FRotator UUnitNavMovementComponent::GetAvoidanceRotation()
{

    AvoidanceScanRunningTime = 0.0f;

    AActor* Owner = GetOwner();
    if (!Owner) {
        return FRotator::ZeroRotator;
    }

    // Get the current location and forward vector of the actor
    FVector CurrentLocation = Owner->GetActorLocation();
    FVector ForwardVector = Owner->GetActorForwardVector();

    // Calculate the end location of the line trace
    FVector TraceEnd = CurrentLocation + (ForwardVector * ScanAheadDistance);

    // Perform the line trace (raycast)
    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(Owner); // Ignore self in trace

    // Optionally, draw debug lines to visualize the trace
    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, CurrentLocation, TraceEnd, ECC_Visibility, CollisionParams);
    /* DrawDebugLine(GetWorld(), CurrentLocation, TraceEnd, FColor::Green, false, AvoidanceScanInterval, 0, 2.0f);
     DrawDebugSphere(GetWorld(), CurrentLocation, 10.0f, 16, FColor::Black, false, AvoidanceScanInterval);
     DrawDebugSphere(GetWorld(), TraceEnd, 10.0f, 16, FColor::Magenta, false, AvoidanceScanInterval);*/

    // Check if we hit something
    if (bHit) {

        GEngine->AddOnScreenDebugMessage(99, AvoidanceScanInterval, FColor::Magenta, TEXT("HIT"));

        // Optionally, draw debug point at the hit location
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 16, FColor::Red, false, AvoidanceScanInterval);

        // Calculate the avoidance direction (rotate away from the hit normal)
        FVector AvoidanceDirection = UKismetMathLibrary::GetReflectionVector(ForwardVector, HitResult.ImpactNormal);
        FRotator AvoidanceRotation = AvoidanceDirection.Rotation();

        // Optionally, you could scale the avoidance rotation by a strength factor
        AvoidanceRotation.Yaw *= 1;

        return AvoidanceRotation;
    }

    // No hit detected, return zero rotation (no avoidance necessary)
    return FRotator::ZeroRotator;
}

// INTERFACES TO AI CONTROLLER
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
FRotator UUnitNavMovementComponent::GetAICommandedRotation()
{
    AAIController* Controller = Cast<AAIController, AController>(OwningUnit->GetController());
    if (IsValid(Controller)) {

        return Controller->GetDesiredRotation();
    }

    return FRotator::ZeroRotator;
}

float UUnitNavMovementComponent::GetCommandedAndActualYawVariance(FRotator& Commanded, FRotator& Actual)
{
    return (Commanded - Actual).Yaw;
}

// POSITION CHECKERS
bool UUnitNavMovementComponent::ShouldBrake(float DeltaTime)
{
    FVector CurrentVelocity = CachedVelocity;
    float CurrentSpeed = CurrentVelocity.Size();
    // suvat equations
    float StoppingDistance = FMath::Pow(CurrentSpeed, 2) / (2 * FMath::Abs(UnitAcceleration));

    // include the "near side" of the acceptance radius
    return StoppingDistance >= (GetRemainingPathLength() - GetAcceptanceRadius());
}
bool UUnitNavMovementComponent::ActorIsOnLastLeg()
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

void UUnitNavMovementComponent::HandleGroundInteraction(float DeltaTime)
{
    FVector CurrentPosition = UpdatedComponent->GetComponentLocation();
    FVector DownLocation = FVector(0, 0, -1);
    FVector LineTraceEnd = CurrentPosition + (DistanceToScanDown * DownLocation);

    FHitResult LineTraceResult;

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());

    if (GetWorld()->LineTraceSingleByChannel(LineTraceResult, CurrentPosition, LineTraceEnd, ECC_Visibility, CollisionParams)) {

        FVector IdealLocation = LineTraceResult.Location;
        IdealLocation.Z += 15.0f;
        UpdatedComponent->SetWorldLocation(IdealLocation);

        DrawDebugSphere(GetWorld(), LineTraceResult.ImpactPoint, 10.0f, 16, FColor::Red, false, AvoidanceScanInterval);
        FRotator NewRotation = IdealLocation.Rotation();
        UpdatedComponent->SetRelativeRotation(NewRotation);
    }
}
