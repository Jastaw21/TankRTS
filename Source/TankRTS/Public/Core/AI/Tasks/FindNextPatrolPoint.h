// Copyright Jack Wedell 2024

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "FindNextPatrolPoint.generated.h"

/**
 * 
 */
UCLASS()
class TANKRTS_API UFindNextPatrolPoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UFindNextPatrolPoint();


private:

	virtual EBTNodeResult::Type ExecuteTask( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory ) override;
	virtual FString GetStaticDescription() const override;
};
