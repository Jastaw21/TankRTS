// Copyright Jack Wedell 2024

#pragma once

#include "BehaviorTree/BTService.h"
#include "CoreMinimal.h"
#include "Core/AI/UnitAIStatus.h"


//gen
#include "RTSUpdateUnitStatus.generated.h"

/**
 Returns the controlled unit's status as enum
 */
UCLASS()
class TANKRTS_API URTSUpdateUnitStatus : public UBTService {
    GENERATED_BODY()

    virtual void TickNode( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds ) override;
    
    UPROPERTY(EditAnywhere)
    FBlackboardKeySelector SelfActorKey;

    UPROPERTY(EditAnywhere)
    FBlackboardKeySelector StatusEnum;

   
};
