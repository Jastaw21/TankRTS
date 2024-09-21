// Copyright Jack Wedell 2024

#include "TankRTS/Public/Core/AI/Tasks/RTSEndMoveTo.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Core/AI/UnitAIStatus.h"

URTSEndMoveTo::URTSEndMoveTo()
{
    NodeName = TEXT("End Move Task - Switch Enum");
}

EBTNodeResult::Type URTSEndMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController { OwnerComp.GetAIOwner() };

    if (AIController) {
        AIController->GetBlackboardComponent()->SetValueAsEnum(FName("UnitStatus"), UnitAIStatus::Rest);
        GEngine->AddOnScreenDebugMessage( 119, 1.0f, FColor::Black, TEXT( "Ending Movmeent" ) );

        return EBTNodeResult::Succeeded;
    }

    else {
        return EBTNodeResult::Failed;
    }
}
