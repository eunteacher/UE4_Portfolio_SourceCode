#include "BehaviorTree/CBTTaskNode_Speed.h"
#include "Global.h"

#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"

#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"

UCBTTaskNode_Speed::UCBTTaskNode_Speed()
{
	NodeName = "Speed";
}

EBTNodeResult::Type UCBTTaskNode_Speed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	UCStatusComponent* status = CHelpers::GetComponent<UCStatusComponent>(ai);
	if (status == NULL)
	{
		return EBTNodeResult::Failed;
	}

	if (status->GetCanMove() == false)
	{
		//CLog::Print("CanMove False");
		return EBTNodeResult::Failed;
	}

	if (state->IsGuarding() == true)
	{
		status->SetSpeed(ECharacterSpeed::Walk);
	}
	else
	{
		status->SetSpeed(SpeedType);
	}
	
	return EBTNodeResult::Succeeded;
}
