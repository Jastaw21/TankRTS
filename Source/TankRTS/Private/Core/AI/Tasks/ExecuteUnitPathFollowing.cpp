// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/AI/Tasks/ExecuteUnitPathFollowing.h"

#include "Tasks/AITask_MoveTo.h"

UExecuteUnitPathFollowing::UExecuteUnitPathFollowing()
{
    NodeName = TEXT("Follow the Path");
}
EBTNodeResult::Type UExecuteUnitPathFollowing::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

    if (GEngine) {
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, TEXT("Executing Path Following"));
    }

    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    return EBTNodeResult::Succeeded;
}

FString UExecuteUnitPathFollowing::GetStaticDescription() const
{
    return FString::Printf(TEXT("Vector: %s"), *BlackboardKey.SelectedKeyName.ToString());
}
