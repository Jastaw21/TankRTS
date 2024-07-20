// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "CoreMinimal.h"
// generated seperator
#include "MoveToRequestedLocation.generated.h"

/**
 * TankRTS Task - To Respond to the players click
 */

class UNiagaraSystem;
class UAITask_MoveTo;
class UBlackboardComponent;


struct FBTMoveToTaskMemory;

UCLASS( config = Game)
class TANKRTS_API UMoveToRequestedLocation : public UBTTask_BlackboardBase {
    GENERATED_BODY()

public:
    UMoveToRequestedLocation();

    // the system that will spawn to indicate the "Path" location
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UNiagaraSystem* FXCursor;

    UPROPERTY(config, Category = Node, EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0"))
    float AcceptableRadius;

private:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual FString GetStaticDescription() const override;

    class UNavigationSystemV1* GetNavSystem();
    class AUnitBase* GetUnit(class AAIController*);
    FVector GetDestination(AAIController* ControllerIn);

    bool CheckPreparationForMove(UBehaviorTreeComponent& OwnerComp);
    EBTNodeResult::Type ExecuteMove(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
    void SetMoveParameters(FAIMoveRequest& MoveReq, AAIController* ControllerIn);
    UAITask_MoveTo* PrepareMoveTask(UBehaviorTreeComponent& OwnerComp, UAITask_MoveTo* ExistingTask, FAIMoveRequest& MoveRequest);
};
