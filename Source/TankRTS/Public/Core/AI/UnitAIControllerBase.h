// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UnitAIControllerBase.generated.h"

/**
 * 
 */


class UBehaviorTreeComponent;
class UBlackboardComponent;


UCLASS()
class TANKRTS_API AUnitAIControllerBase : public AAIController
{
	GENERATED_BODY()

public:
	AUnitAIControllerBase();

	void LogRequest();
	void Speak();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY( EditDefaultsOnly,BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = true))
	TObjectPtr<UBehaviorTree> BehaviourTree;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = true) )
	TObjectPtr<UBehaviorTreeComponent> BehaviourTreeComponent;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = true) )
	TObjectPtr<UBlackboardComponent> BlackBoardComponent;



};
