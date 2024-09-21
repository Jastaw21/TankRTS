// Copyright Jack Wedell 2024

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "RTSEndMoveTo.generated.h"

/**
 * 
 */
UCLASS()
class TANKRTS_API URTSEndMoveTo : public UBTTask_BlackboardBase
{
	GENERATED_BODY()


public:
	URTSEndMoveTo();

private:
	virtual EBTNodeResult::Type ExecuteTask( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory ) override;

	
};
