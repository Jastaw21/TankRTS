// Copyright Jack Wedell 2024

#include "Core/AI/Tasks/BeginMoveTask.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Core/AI/UnitAIStatus.h"
#include "Core/Units/Base/UnitBase.h"

UBeginMoveTask::UBeginMoveTask()
{
    NodeName = TEXT("Set-off Movement");
    BlackboardKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBeginMoveTask, BlackboardKey.SelectedKeyName));
}

EBTNodeResult::Type UBeginMoveTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

    EBTNodeResult::Type NodeResult = EBTNodeResult::Failed;

    AAIController* AIController { OwnerComp.GetAIOwner() };

    if (AIController) {
        APawn* AIPawn { AIController->GetPawn() };
        if (AIPawn) {
            AUnitBase* Unit = Cast<AUnitBase, APawn>(AIPawn);

            Unit->SetIsUnitMoving(true);
            Unit->Execute_SetStatus(Unit, UnitAIStatus::MovingTo);

            NodeResult = EBTNodeResult::Succeeded;
        }
    }

    return NodeResult;
}

FString UBeginMoveTask::GetStaticDescription() const
{
    return FString(TEXT("DD"));
}
