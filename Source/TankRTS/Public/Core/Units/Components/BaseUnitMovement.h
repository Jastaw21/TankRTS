// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TankRTS/Public/Core/Utility/DesiredMovementDetails.h"
#include "GameFramework/PawnMovementComponent.h"
/**
 *
 */
#include "BaseUnitMovement.generated.h"

class AUnitBase;

UCLASS()
class TANKRTS_API UBaseUnitMovement : public UPawnMovementComponent {
    GENERATED_BODY()

public:
    UBaseUnitMovement();

    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    void SetUpdatedComponentLocal(AUnitBase* OwningComponent_p);

     
    

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Variables")
    float MovementSpeed = 200.0f;

private:
    AUnitBase* OwningUnit;
    bool IsAlreadyNearLocation(FVector& DesiredLocation);
};
