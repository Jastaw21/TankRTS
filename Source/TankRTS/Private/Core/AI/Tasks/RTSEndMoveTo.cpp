// Copyright Jack Wedell 2024

#include "TankRTS/Public/Core/AI/Tasks/RTSEndMoveTo.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Core/AI/UnitAIStatus.h"
#include "Core/Units/Base/UnitBase.h"

URTSEndMoveTo::URTSEndMoveTo()
{
    NodeName = TEXT("End Move Task - Switch Enum");
}

EBTNodeResult::Type URTSEndMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController { OwnerComp.GetAIOwner() };

    if (AIController) {

        APawn* AIPawn { AIController->GetPawn() };
        if (AIPawn) {
            AUnitBase* Unit = Cast<AUnitBase, APawn>(AIPawn);
            Unit->Execute_SetStatus(Unit, UnitAIStatus::Rest);
            return EBTNodeResult::Succeeded;
        }
    }

    return EBTNodeResult::Failed;
}
