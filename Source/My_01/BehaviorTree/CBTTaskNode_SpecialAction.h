#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_SpecialAction.generated.h"

UCLASS()
class MY_01_API UCBTTaskNode_SpecialAction : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCBTTaskNode_SpecialAction();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
