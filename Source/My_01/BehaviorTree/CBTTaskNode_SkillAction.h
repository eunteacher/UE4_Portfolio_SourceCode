#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_SkillAction.generated.h"

UCLASS()
class MY_01_API UCBTTaskNode_SkillAction : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "UserSetting")
		int32 SkillIndex = 0;

	UPROPERTY(EditAnywhere, Category = "UserSetting")
		bool SelectSkill = false;

public:
	UCBTTaskNode_SkillAction();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
