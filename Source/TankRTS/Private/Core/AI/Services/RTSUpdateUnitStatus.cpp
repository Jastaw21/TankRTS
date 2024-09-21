// Copyright Jack Wedell 2024

#include "Core/AI/Services/RTSUpdateUnitStatus.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Core/Units/Base/UnitBase.h"

#include "AIController.h"

void URTSUpdateUnitStatus::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

    AAIController* Controller = OwnerComp.GetAIOwner();

    if (Controller)

    {
        APawn* ControlledPawn = Controller->GetPawn();

        if (ControlledPawn) {
            AUnitBase* Unit = Cast<AUnitBase, APawn>(ControlledPawn);
            UnitAIStatus Status = Unit->Execute_GetUnitStatus(Unit);

            OwnerComp.GetBlackboardComponent()->SetValueAsEnum(StatusEnum.SelectedKeyName, Status);
        }
    }
}
