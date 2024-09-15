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
    Velocity = MoveInput;
}

void UUnitNavMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

    if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime)) {
        return;
    }

    else {

        if (Velocity.Length() == 0) {
            GEngine->AddOnScreenDebugMessage(1, DeltaTime * 1.1, FColor::Black, TEXT("NO MOVEMENT REQUIRED"));
        }

        else {
            GEngine->AddOnScreenDebugMessage(1, DeltaTime * 1.1, FColor::White, TEXT("MOVEMENT REQUESTED"));

            DrawPathLines();

            FRotator NewRotation = GetVelocityRotator(DeltaTime);
            FVector NewDirection;

            if (FMath::IsNearlyZero(GetCommandedAndActualYawVariance(GetAICommandedRotation(), NewRotation), RotationTolerance*2)) {

                GEngine->AddOnScreenDebugMessage(2, DeltaTime * 1.1, FColor::Green, TEXT("Rotation Aligned"));
                NewDirection = GetNewVelocity(DeltaTime);
            }

            else {
                GEngine->AddOnScreenDebugMessage(4, DeltaTime * 1.1, FColor::Red, TEXT("Rotation Mis-Aligned"));
                NewDirection = FVector::ZeroVector;
            }

            FHitResult Hit;

            if (!SafeMoveUpdatedComponent(NewDirection, NewRotation, true, Hit))

            {
                GEngine->AddOnScreenDebugMessage(3, DeltaTime * 1.1, FColor::Red, TEXT("Colliding with Something"));

                SlideAlongSurface(NewDirection, 1.0f - Hit.Time, Hit.Normal, Hit);
            }

            else {
                GEngine->AddOnScreenDebugMessage(3, DeltaTime * 1.1, FColor::Green, TEXT("No collison, OK to move"));
                MoveUpdatedComponent(NewDirection, NewRotation, false);
            }

            CachedVelocity = NewDirection;

            HandleGroundInteraction(DeltaTime);
        }
    }
}

// NEXT MOVE CALCULATORS
FRotator UUnitNavMovementComponent::GetVelocityRotator(float DeltaTime)
{

    FRotator RawVelocRotation = Velocity.ToOrientationRotator();

    FRotator DeltaRot = RawVelocRotation - PawnOwner->GetActorRotation();

    // if we're within the rotation tolerance - early out
    if (DeltaRot.IsNearlyZero(RotationTolerance *0.5)) {

        return PawnOwner->GetActorRotation();
    }

    else {

        JWMath::ScaleVectorToSpeed(DeltaRot, RotationSpeed * DeltaTime);
        DeltaRot.Pitch = 0;
        DeltaRot.Roll = 0;
        // add the new calculated incremental rotation to the existing one
        return PawnOwner->GetActorRotation() + (DeltaRot);
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

float UUnitNavMovementComponent::GetCommandedAndActualYawVariance(const FRotator& Commanded, const FRotator& Actual)
{
    return (Commanded - Actual).Yaw;
}

void UUnitNavMovementComponent::DrawPathLines()
{
    AAIController* Controller = Cast<AAIController, AController>(OwningUnit->GetController());

    if (IsValid(Controller)) {

        UPathFollowingComponent* PFComponent = Controller->GetPathFollowingComponent();

        if (PFComponent != nullptr && PFComponent->HasValidPath()) {

            const FNavPathSharedPtr PathToFollow = PFComponent->GetPath();

            FVector LastPathPoint = UpdatedComponent->GetComponentLocation();
            uint32 LastPPIndex = PFComponent->GetCurrentPathIndex();

            int32 Length = PathToFollow->GetPathPoints().Num();

            for (FNavPathPoint& PathPoint : PathToFollow->GetPathPoints()) {

                DrawDebugSphere(GetWorld(), PathPoint.Location, 20.0f, 32, FColor::Turquoise, false, 0.2f);
            }
        }
    }
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

    // ingore ourselves in the query
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    float DistanceToScan = 100.0f;
    UBoxComponent* ForeBox = OwningUnit->GetForwardCollisionBox();
    UBoxComponent* AftBox = OwningUnit->GetAftCollisionBox();

    // scan the front first
    FHitResult FwdLineTraceResult;
    FVector FwdCollisionLocation = ForeBox->GetComponentLocation();

    // get a direction downwards from the collision box
    FRotator FwdCollisionRotation = (ForeBox->GetUpVector().Rotation()) *= -1;

    // scan 100 units in the dwds direction
    FVector FwdScanTo = FwdCollisionLocation + FwdCollisionRotation.Vector() * DistanceToScan;

    // perform the scan & draw a debug line
    bool FwdColl = GetWorld()->LineTraceSingleByChannel(FwdLineTraceResult, FwdCollisionLocation, FwdScanTo, ECC_Visibility, CollisionParams);
    DrawDebugLine(GetWorld(), FwdCollisionLocation, FwdScanTo, FColor::Cyan, false, GroundScanInterval);

    // scan the back
    FHitResult AftLineTraceResult;
    FVector AftCollisionLoc = AftBox->GetComponentLocation();

    // get a direction downwards from the collision box
    FRotator AftCollisionRot = (AftBox->GetUpVector().Rotation()) *= -1;

    // scan 100 units in the dwds direction
    FVector AftScanTo = AftCollisionLoc + AftCollisionRot.Vector() * DistanceToScan;

    // perform the scan & draw a debug line
    bool AftColl = GetWorld()->LineTraceSingleByChannel(AftLineTraceResult, AftCollisionLoc, AftScanTo, ECC_Visibility, CollisionParams);
    DrawDebugLine(GetWorld(), AftCollisionLoc, AftScanTo, FColor::Red, false, GroundScanInterval);

    if (FwdColl && AftColl) {

        // take the average Z position of the two scans - and prevent us from going underground
        float ZLoc = FMath::Clamp((FwdLineTraceResult.ImpactPoint.Z + AftLineTraceResult.ImpactPoint.Z) / 2.0f, 5.0f, 2000.0f);
        FVector CurrentLocation = UpdatedComponent->GetComponentLocation();

        // only edit the Z position, to snap towards floor
        CurrentLocation.Z = ZLoc + 20.0f;

        UpdatedComponent->SetRelativeLocation(CurrentLocation);

        // find the angle of the line between the two collisions
        FRotator BodyRotation = (FwdLineTraceResult.ImpactPoint - AftLineTraceResult.ImpactPoint).Rotation();

        UpdatedComponent->SetRelativeRotation(BodyRotation);
    }
}
