// Fill out your copyright notice in the Description page of Project Settings.

#include "TankRTS/Public/Core/AI/Tasks/UnitAITaskFindLoc.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Core/Units/Base/UnitBase.h"
#include "Core/Units/Components/UnitNavMovementComponent.h"
#include "NavigationSystem.h"

UUnitAITaskFindLoc::UUnitAITaskFindLoc()
{
    NodeName = TEXT("Find Random Location");
    BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UUnitAITaskFindLoc, BlackboardKey));
}

EBTNodeResult::Type UUnitAITaskFindLoc::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // evventually will be populated with where to go
    FNavLocation Location {};

    // goes off to find the AI controller
    AAIController* AIController { OwnerComp.GetAIOwner() };

    // will be populated with starting point of pawn
    FVector Origin;

    if (AIController) // check AI controller exists
    {

        // get a raw pawn ref
        APawn* AIPawn { AIController->GetPawn() };        
        if (AIPawn) {

          

            Origin = AIPawn->GetActorLocation();
            AUnitBase* Unit = Cast<AUnitBase, APawn>(AIPawn);

            const UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

            if (IsValid(NavSystem) && NavSystem->GetRandomPointInNavigableRadius(Origin, SearchRadius, Location)) {
                AIController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, Location.Location);
            }       

            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
            return EBTNodeResult::Succeeded;
        }

        else {

            FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
            return EBTNodeResult::Failed;
        }
    }

    else {       

        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return EBTNodeResult::Failed;
    }
}

FString UUnitAITaskFindLoc::GetStaticDescription() const
{
    return FString::Printf(TEXT("Vector: %s"), *BlackboardKey.SelectedKeyName.ToString());
}
