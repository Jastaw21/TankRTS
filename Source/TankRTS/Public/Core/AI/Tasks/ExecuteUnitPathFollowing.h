// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "CoreMinimal.h"
#include "ExecuteUnitPathFollowing.generated.h"


/**
 *
 */
UCLASS()
class TANKRTS_API UExecuteUnitPathFollowing : public UBTTask_BlackboardBase {
    GENERATED_BODY()

public:
    UExecuteUnitPathFollowing();

private:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual FString GetStaticDescription() const override;
};
