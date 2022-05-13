#include "BehaviorTree/CBTTaskNode_Guard.h"
#include "Global.h"

#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"

#include "Components/CStateComponent.h"

UCBTTaskNode_Guard::UCBTTaskNode_Guard()
{
	NodeName = "Guard";
}

EBTNodeResult::Type UCBTTaskNode_Guard::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (controller == NULL)
	{
		return EBTNodeResult::Failed;
	}

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	if (ai == NULL)
	{
		return EBTNodeResult::Failed;
	}

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	if (state == NULL)
	{
		return EBTNodeResult::Failed;
	}

	if (state->IsDead() == true)
	{
		return EBTNodeResult::Failed;
	}

	state->SetGuardingMode();

	if (state->IsGuarding() == false)
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;
}
