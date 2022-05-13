#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Avoid.generated.h"

UCLASS()
class MY_01_API UCBTTaskNode_Avoid : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCBTTaskNode_Avoid();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
