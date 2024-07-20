// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/AI/Tasks/MoveToRequestedLocation.h"
#include "AIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Runtime/NavigationSystem/Public/NavigationPath.h"
#include "TankRTS/Public/Core/Units/Base/UnitBase.h"
#include "Tasks/AITask_MoveTo.h"

struct FBTMoveToTaskMemory{
    /** Move request ID */
    FAIRequestID MoveRequestID;

    FDelegateHandle BBObserverDelegateHandle;
    FVector PreviousGoalLocation;

    TWeakObjectPtr<UAITask_MoveTo> Task;

    uint8 bObserverCanFinishTask : 1;
    };


UMoveToRequestedLocation::UMoveToRequestedLocation()
{
    NodeName = TEXT("Build and Execute Navigation");
    BlackboardKey.AddVectorFilter(this,
        GET_MEMBER_NAME_CHECKED(UMoveToRequestedLocation, BlackboardKey));
    FXCursor = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Game/TankRTS/FX/PathMarking/PathMarker.PathMarker"));
}

// this is called by the AI tree externally
EBTNodeResult::Type UMoveToRequestedLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

    // placeholder return type
    EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;
    FBTMoveToTaskMemory* MyMemory = CastInstanceNodeMemory<FBTMoveToTaskMemory>(NodeMemory);
    MyMemory->MoveRequestID = FAIRequestID::InvalidRequest;

    AAIController* MyAIController;
    AUnitBase* MyUnit;
    UNavigationSystemV1* MyNavSystem;

    if (!CheckPreparationForMove(OwnerComp)) {
        NodeResult = EBTNodeResult::Failed;
    }

    else {

        MyAIController = OwnerComp.GetAIOwner();
        MyUnit = GetUnit(MyAIController);
        MyNavSystem = GetNavSystem();

        if (IsValid(MyNavSystem) && MyUnit->IsMoveable()) {

            ExecuteMove(OwnerComp, NodeMemory);
        }
    }

    return NodeResult;
}

EBTNodeResult::Type UMoveToRequestedLocation::ExecuteMove(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
    FBTMoveToTaskMemory* MyMemory = CastInstanceNodeMemory<FBTMoveToTaskMemory>(NodeMemory);
    AAIController* MyController = OwnerComp.GetAIOwner();

    EBTNodeResult::Type NodeResult = EBTNodeResult::Failed;
    if (MyController && MyBlackboard) {

        FAIMoveRequest MoveReq;
        SetMoveParameters(MoveReq, MyController);

        if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass()) {

            // poll the blackboard for the destination
            const FVector TargetLocation = MyBlackboard->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());

            // pass it into the move
            MoveReq.SetGoalLocation(TargetLocation);
            // cache it in the memory
            MyMemory->PreviousGoalLocation = TargetLocation;
        }

        // final check - then proceed
        if (MoveReq.IsValid()) {

            // get the move task existing
            UAITask_MoveTo* MoveTask = MyMemory->Task.Get();
            const bool bReuseExistingTask = (MoveTask != nullptr);

            MoveTask = PrepareMoveTask(OwnerComp, MoveTask, MoveReq);

            if (MoveTask) {
                MyMemory->bObserverCanFinishTask = false;

                if (bReuseExistingTask) {
                    if (MoveTask->IsActive()) {
                        MoveTask->ConditionalPerformMove();
                    }
                }

                else {
                    MyMemory->Task = MoveTask;
                    
                }
                MyMemory->bObserverCanFinishTask = true;
                NodeResult = (MoveTask->GetState() != EGameplayTaskState::Finished) ? EBTNodeResult::InProgress : MoveTask->WasMoveSuccessful() ? EBTNodeResult::Succeeded
                                                                                                                                                : EBTNodeResult::Failed;
            }
        }
    }


    return NodeResult;
}

// pulled out some of the branching logic to make life cleaner in the execute function
bool UMoveToRequestedLocation::CheckPreparationForMove(UBehaviorTreeComponent& OwnerComp)
{

    bool CanProceedWithMove = true;

    // if we get an invalid controller - exit early
    AAIController* AIController { OwnerComp.GetAIOwner() };
    if (!IsValid(AIController)) {
        CanProceedWithMove = false;
        return CanProceedWithMove;
    }

    // then, if we get an invalid unit - exit early
    AUnitBase* ControlledUnit = GetUnit(AIController);
    if (!IsValid(ControlledUnit)) {
        CanProceedWithMove = false;
        return CanProceedWithMove;
    }

    return CanProceedWithMove;
}

UAITask_MoveTo* UMoveToRequestedLocation::PrepareMoveTask(UBehaviorTreeComponent& OwnerComp, UAITask_MoveTo* ExistingTask, FAIMoveRequest& MoveRequest)
{
    UAITask_MoveTo* MoveTask = ExistingTask ? ExistingTask : NewBTAITask<UAITask_MoveTo>(OwnerComp);
    if (MoveTask) {
        MoveTask->SetUp(MoveTask->GetAIController(), MoveRequest);
    }

    return MoveTask;
}

// getters etc
FString UMoveToRequestedLocation::GetStaticDescription() const
{
    return FString::Printf(TEXT("Vector: %s"), *BlackboardKey.SelectedKeyName.ToString());
}
UNavigationSystemV1* UMoveToRequestedLocation::GetNavSystem()
{
    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
    return NavSystem;
}
AUnitBase* UMoveToRequestedLocation::GetUnit(AAIController* ControllerIn)
{
    return Cast<AUnitBase, APawn>(ControllerIn->GetPawn());
}
FVector UMoveToRequestedLocation::GetDestination(AAIController* ControllerIn)
{
    return ControllerIn->GetBlackboardComponent()->GetValueAsVector(BlackboardKey.SelectedKeyName);
}
void UMoveToRequestedLocation::SetMoveParameters(FAIMoveRequest& MoveReq, AAIController* ControllerIn)
{
    MoveReq.SetAcceptanceRadius(AcceptableRadius);
    MoveReq.SetNavigationFilter(ControllerIn->GetDefaultNavigationFilterClass());
    MoveReq.SetAllowPartialPath(1);
    MoveReq.SetCanStrafe(1);
    MoveReq.SetReachTestIncludesAgentRadius(1);
    MoveReq.SetReachTestIncludesGoalRadius(1);
    MoveReq.SetRequireNavigableEndLocation(1);
    MoveReq.SetProjectGoalLocation(1);
    MoveReq.SetUsePathfinding(1);
}
