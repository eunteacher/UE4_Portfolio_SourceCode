#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Equip.generated.h"

UCLASS()
class MY_01_API UCBTTaskNode_Equip : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCBTTaskNode_Equip();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
