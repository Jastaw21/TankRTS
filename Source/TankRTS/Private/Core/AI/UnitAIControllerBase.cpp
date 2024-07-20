// Fill out your copyright notice in the Description page of Project Settings.

#include "TankRTS/Public/Core/AI/UnitAIControllerBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Navigation/PathFollowingComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TankRTS/Public/Core/AI/Tasks/UnitAITaskFindLoc.h"
#include "TankRTS/Public/Core/Units/Base/UnitBase.h"

AUnitAIControllerBase::AUnitAIControllerBase()
{
    BehaviourTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behaviour Tree Comp"));
    BlackBoardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));
   
}

void AUnitAIControllerBase::LogRequest()
{
    UE_LOG(LogTemp, Display, TEXT("AI CONTROLLER CALLED"));
}

void AUnitAIControllerBase::Speak()
{
    UE_LOG(LogTemp, Display, TEXT("AI CONTROLLER SPEAKING"));
}

void AUnitAIControllerBase::BeginPlay()
{
    Super::BeginPlay();

    if (IsValid(BehaviourTree.Get())) {
        RunBehaviorTree(BehaviourTree.Get());
        BehaviourTreeComponent->StartTree(*BehaviourTree.Get());
    }
}

void AUnitAIControllerBase::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (InPawn == nullptr) {
        UE_LOG(LogTemp, Display, TEXT("JJJAA"));
    }

    AUnitBase* Unit = Cast<AUnitBase, APawn>(InPawn);   
       

    if (IsValid(Blackboard.Get()) && IsValid(BehaviourTree.Get()))

    {
        Blackboard->InitializeBlackboard(*BehaviourTree.Get()->BlackboardAsset.Get());
    }
}
