// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/Units/Components/UnitNavMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Core/Game Mode/RTSGameState.h"
#include "Core/Units/Base/UnitBase.h"
#include "Core/Utility/JWMath.h"
#include "GameFramework/GameState.h"
#include "Navigation/PathFollowingComponent.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"

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

// called in the main game loop - all movement handling flows through here
void UUnitNavMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

    if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime)) {
        return;
    }

    else {

        LineTraceRunningTime += DeltaTime;
        bool ShouldLineTrace = LineTraceRunningTime >= LineTraceInterval;
        FRotator LineTracedRotator = FRotator::ZeroRotator;

        if (ShouldLineTrace) {
            FVector CurrentLoc = OwningUnit->GetActorLocation();
            LineTraceRunningTime = 0.0f;
            LineTracedRotator = LineTraceXSecondsAhead(CurrentLoc, LineTraceInterval);
        }

        FVector Delta = GetNewVelocity(DeltaTime);
        FRotator NewRotation = GetVelocityRotator(DeltaTime) + LineTracedRotator;
        FHitResult HitRes;

        bool SafeMoved = SafeMoveUpdatedComponent(Delta, NewRotation, true, HitRes, ETeleportType::TeleportPhysics);

        if (HitRes.bBlockingHit) {
            SlideAlongSurface(Delta, 1 - HitRes.Time, HitRes.ImpactNormal, HitRes);
        }

        CachedVelocity = Delta;
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

FRotator UUnitNavMovementComponent::LineTraceXSecondsAhead(FVector& CurrentLocation_p, float SecondsTilNextTick)
{

    // will be worth moving to a UPROPERTY later
    float ZDistanceToScan = 10.0f;

    // where is the forward collision box
    FVector ForwardCollisionBoxLocation = OwningUnit->GetForwardCollisionBox()->GetComponentLocation();

    // calculate the scan ahead point
    FVector NextLocation = (ForwardCollisionBoxLocation + (OwningUnit->GetActorForwardVector() * DistanceAheadToTrack * SecondsTilNextTick));
    FVector ScanLoc = NextLocation - FVector(0, 0, ZDistanceToScan);

    // calculate the scan below pointt
    FVector ScanBelowLoc = ForwardCollisionBoxLocation - FVector(0, 0, ZDistanceToScan *5);

    /*

  DRAW DEBUG LINES

  */
    // draw the line ahead
    DrawDebugLine(GetWorld(), ForwardCollisionBoxLocation, NextLocation, FColor::Black, false, LineTraceInterval);
    // draw sphere at current loc
    DrawDebugSphere(GetWorld(), ForwardCollisionBoxLocation, 10.0f, 32, FColor::Green, false, LineTraceInterval);
    // draw sphere at calculated next loc
    DrawDebugSphere(GetWorld(), NextLocation, 10.0f, 32, FColor::Red, false, LineTraceInterval);
    DrawDebugSphere(GetWorld(), ScanLoc, 10.0f, 32, FColor::Yellow, false, LineTraceInterval);
    DrawDebugSphere(GetWorld(), ForwardCollisionBoxLocation, 10.0f, 32, FColor(230,45,100), false, LineTraceInterval);
    DrawDebugSphere(GetWorld(), ScanBelowLoc, 10.0f, 32, FColor(130,145,200), false, LineTraceInterval);

    // line trace the ahead
    FHitResult LineTraceAheadResult;
    GetWorld()->LineTraceSingleByChannel(LineTraceAheadResult, ForwardCollisionBoxLocation, ScanLoc, ECollisionChannel::ECC_WorldDynamic);

    FHitResult LineTraceBelowResult;
    GetWorld()->LineTraceSingleByChannel(LineTraceBelowResult, ForwardCollisionBoxLocation, ScanBelowLoc, ECollisionChannel::ECC_WorldDynamic);

    if (LineTraceAheadResult.bBlockingHit || LineTraceBelowResult.bBlockingHit) {

        FRotator ReturnRotator;

        if (!LineTraceBelowResult.bBlockingHit) {
            GEngine->AddOnScreenDebugMessage(1121, 1.0f, FColor::White, TEXT("HIT BELOW"));
            FVector HitLocation = LineTraceAheadResult.Location;

            ReturnRotator = (HitLocation - (ForwardCollisionBoxLocation)).ToOrientationRotator();
            DrawDebugSphere(GetWorld(), LineTraceAheadResult.ImpactPoint, 80.0f, 16, FColor::Turquoise, true, 3.0f);

            return ReturnRotator;

        }

        else if (LineTraceAheadResult.bBlockingHit) {

            GEngine->AddOnScreenDebugMessage(111, 1.0f, FColor::Red, TEXT("HIT"));
            FVector HitLocation = LineTraceAheadResult.Location;

            ReturnRotator = (HitLocation - (ForwardCollisionBoxLocation)).ToOrientationRotator();
            DrawDebugSphere(GetWorld(), LineTraceAheadResult.ImpactPoint, 80.0f, 16, FColor::Turquoise, true, 3.0f);

            return ReturnRotator;
        }
    }

    return FRotator::ZeroRotator;
}

FRotator UUnitNavMovementComponent::GetVelocityRotator(float DeltaTime)
{

    FRotator RawAICommandedRotation = GetAICommandedRotation();
    FRotator RawVelocRotation = Velocity.ToOrientationRotator();
    FRotator BlendedRotation = FRotator(RawAICommandedRotation.Pitch, RawAICommandedRotation.Yaw, 0.0f);

    FRotator DeltaRot = BlendedRotation - PawnOwner->GetActorRotation();

    // if we're within the rotation tolerance - early out
    if (DeltaRot.IsNearlyZero(RotationTolerance)) {
        return PawnOwner->GetActorRotation();
    }

    JWMath::ScaleVectorToSpeed(DeltaRot, RotationSpeed * DeltaTime);
    DeltaRot.Roll = 0.00f;

    // add the new calculated incremental rotation to the existing one
    return PawnOwner->GetActorRotation() + DeltaRot;
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
    return ClampedNewSpeed * NormalisedRequestedVelocity;
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
FRotator UUnitNavMovementComponent::GetAICommandedRotation()
{
    AAIController* Controller = Cast<AAIController, AController>(OwningUnit->GetController());
    if (IsValid(Controller)) {

        return Controller->GetDesiredRotation();
    }

    return FRotator::ZeroRotator;
}

void UUnitNavMovementComponent::PushRotatorToUI(FRotator& inRotator)
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



#include "MyCustomMovementComponent.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"

UMyCustomMovementComponent::UMyCustomMovementComponent()
{
    // Initialize default values
    AvoidanceScanInterval = 0.5f;  // Scan every 0.5 seconds
    TimeSinceLastScan = 0.0f;
}

void UMyCustomMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Accumulate time since the last scan
    TimeSinceLastScan += DeltaTime;

    // Only scan if enough time has passed
    if (TimeSinceLastScan >= AvoidanceScanInterval)
    {
        // Perform the avoidance check and reset the timer
        FRotator AvoidanceRotation = GetAvoidanceRotation();

        // Apply the avoidance rotation if needed
        if (!AvoidanceRotation.IsNearlyZero())
        {
            AActor* Owner = GetOwner();
            if (Owner)
            {
                FRotator NewRotation = Owner->GetActorRotation() + AvoidanceRotation;
                Owner->SetActorRotation(NewRotation);
            }
        }

        TimeSinceLastScan = 0.0f;  // Reset the scan timer
    }

    // Perform your regular movement logic here, e.g., SafeMoveUpdatedComponent()
    // SafeMoveUpdatedComponent(DesiredMovement, UpdatedComponent->GetComponentRotation());
}

FRotator UMyCustomMovementComponent::GetAvoidanceRotation()
{
    AActor* Owner = GetOwner();
    if (!Owner)
    {
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
    CollisionParams.AddIgnoredActor(Owner);  // Ignore self in trace

    // Optionally, draw debug lines to visualize the trace
    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, CurrentLocation, TraceEnd, ECC_Visibility, CollisionParams);
    DrawDebugLine(GetWorld(), CurrentLocation, TraceEnd, FColor::Green, false, AvoidanceScanInterval, 0, 2.0f);

    // Check if we hit something
    if (bHit)
    {
        // Optionally, draw debug point at the hit location
        DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.0f, FColor::Red, false, AvoidanceScanInterval);

        // Calculate the avoidance direction (rotate away from the hit normal)
        FVector AvoidanceDirection = UKismetMathLibrary::GetReflectionVector(ForwardVector, HitResult.ImpactNormal);
        FRotator AvoidanceRotation = AvoidanceDirection.Rotation();

        // Optionally, you could scale the avoidance rotation by a strength factor
        AvoidanceRotation.Yaw *= AvoidanceStrength;

        return AvoidanceRotation;
    }

    // No hit detected, return zero rotation (no avoidance necessary)
    return FRotator::ZeroRotator;
}
