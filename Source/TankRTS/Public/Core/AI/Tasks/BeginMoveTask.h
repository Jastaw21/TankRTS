// Copyright Jack Wedell 2024

#pragma once


#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "CoreMinimal.h"
//
//
#include "BeginMoveTask.generated.h"

/**
 Toggles the unit state to active
 */
UCLASS()
class TANKRTS_API UBeginMoveTask : public UBTTask_BlackboardBase {
    GENERATED_BODY()

public:
    UBeginMoveTask();

private:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual FString GetStaticDescription() const override;
};
