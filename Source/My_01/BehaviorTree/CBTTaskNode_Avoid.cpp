#include "BehaviorTree/CBTTaskNode_Avoid.h"
#include "Global.h"

#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"

#include "Components/CStateComponent.h"

UCBTTaskNode_Avoid::UCBTTaskNode_Avoid()
{
	NodeName = "Avoid";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Avoid::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (controller == NULL)
	{
		return EBTNodeResult::Failed;
	}

	UCBehaviorCompoent* behavior = CHelpers::GetComponent<UCBehaviorCompoent>(controller);
	if (behavior == NULL)
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
	
	if (state->IsDead() == true || state->IsFlying() == true || state->IsAvoid() == true)
	{
		return EBTNodeResult::Failed;
	}

	state->SetAvoidMode();

	//CLog::Print("Task SetAvoidMode()");

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Avoid::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (controller == NULL)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	if (ai == NULL)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	if (state == NULL)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	if (state->IsDead() == true)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	if (state->IsIdle() == true)
	{
		//CLog::Log("Succeeded");
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

