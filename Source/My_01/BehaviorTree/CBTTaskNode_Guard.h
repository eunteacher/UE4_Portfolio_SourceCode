#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Guard.generated.h"

UCLASS()
class MY_01_API UCBTTaskNode_Guard : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCBTTaskNode_Guard();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	
};
