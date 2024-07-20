// Fill out your copyright notice in the Description page of Project Settings.

#include "TankRTS/Public/Core/Units/Components/BaseUnitMovement.h"
#include "TankRTS/Public/Core/Units/Base/UnitBase.h"

void UBaseUnitMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime) || !OwningUnit) {
        return;
    }

    // check if there's a destination set
    if (OwningUnit->IsMoveable()) {

        // see if we're nearly there, if so, toggle the moveable flag to off
        if (IsAlreadyNearLocation(OwningUnit->GetMovementDetails().DesiredLocation)) {
            OwningUnit->CancelMovement();

        }

        // otherwise we want to try and move
        else {

            FHitResult hitRes;
            // whats the general direction?
            FVector DesiredMovement = ((OwningUnit->GetMovementDetails().DesiredLocation) - (OwningUnit->GetActorLocation()));

            // Normalise it to 1.
            DesiredMovement = DesiredMovement.GetSafeNormal().GetClampedToMaxSize(1);
            DesiredMovement.Z = 0.0f;
            DesiredMovement.Normalize();

            DesiredMovement *= MovementSpeed * DeltaTime * FVector(1, 1, 0) * (1.0f / DesiredMovement.Length());
            if (DesiredMovement.Length() <= 0.02) {
                OwningUnit->CancelMovement();
            } else {

                // move it
                SafeMoveUpdatedComponent(DesiredMovement,
                    UpdatedComponent->GetRelativeRotation(),
                    true,
                    hitRes);

                if (hitRes.GetActor()) {

                    OwningUnit->CancelMovement();
                   
                }
            }
        }
    }
}

UBaseUnitMovement::UBaseUnitMovement()
{
    if (PawnOwner) {
        OwningUnit = Cast<AUnitBase, APawn>(PawnOwner);
    }
}

void UBaseUnitMovement::SetUpdatedComponentLocal(AUnitBase* OwningComponent_p)
{
    OwningUnit = OwningComponent_p;
    UpdatedComponent = OwningUnit->GetRootComponent();
}

bool UBaseUnitMovement::IsAlreadyNearLocation(FVector& DesiredLocation)
{
    bool isNear = false;

    isNear = FMath::IsNearlyEqual(DesiredLocation.X, UpdatedComponent->GetRelativeLocation().X, 0.2)
        && FMath::IsNearlyEqual(DesiredLocation.Y, UpdatedComponent->GetRelativeLocation().Y, 0.2);

    return isNear;
}
