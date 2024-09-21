// Copyright Jack Wedell 2024

#include "Core/AI/Tasks/FindNextPatrolPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Core/Units/Base/UnitBase.h"
#include "NavigationSystem.h"

#include "AIController.h"

UFindNextPatrolPoint::UFindNextPatrolPoint()
{
    NodeName = TEXT("Find Next Patrol Point");
}

EBTNodeResult::Type UFindNextPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

    AAIController* AIController = OwnerComp.GetAIOwner();

    // if that worked, start using the AIController pointer to find downstream references
    if (AIController) {

        // get, and cast a pointer to our unit
        APawn* AIControlledPawn = AIController->GetPawn();
        AUnitBase* Unit = Cast<AUnitBase, APawn>(AIControlledPawn);

        // if that worked - follow on with the point finding
        if (Unit) {

            // end point of the task - ultimate move to goal
            FNavLocation Location {};

            // current loc
            FVector Origin;

            const UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
            Origin = Unit->GetActorLocation();

            if (IsValid(NavSystem) && NavSystem->GetRandomReachablePointInRadius(Origin, Unit->GetPatrolRadius(), Location)) {
                AIController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, Location);
                return EBTNodeResult::Succeeded;

            }

            else
                return EBTNodeResult::Failed;

        }

        else {
            return EBTNodeResult::Failed;
        }
    }

    else {
        return EBTNodeResult::Failed;
    }
}

FString UFindNextPatrolPoint::GetStaticDescription() const
{
    return FString::Printf(TEXT("Vector: %s"), *BlackboardKey.SelectedKeyName.ToString());
}
