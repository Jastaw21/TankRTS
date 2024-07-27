// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "CoreMinimal.h"
/**
 * Task for finding random loc on nav
 */
#include "UnitAITaskFindLoc.generated.h"

/**
 * Task for finding random loc on nav
 */
UCLASS()
class TANKRTS_API UUnitAITaskFindLoc : public UBTTask_BlackboardBase {
    GENERATED_BODY()

public:
    UUnitAITaskFindLoc();

private:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual FString GetStaticDescription() const override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowPrivateAccess = true))
    float SearchRadius { 10.0f };
};
