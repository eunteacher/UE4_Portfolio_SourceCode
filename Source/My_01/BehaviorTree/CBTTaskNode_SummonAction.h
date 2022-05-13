#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_SummonAction.generated.h"

UCLASS()
class MY_01_API UCBTTaskNode_SummonAction : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCBTTaskNode_SummonAction();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	
};
