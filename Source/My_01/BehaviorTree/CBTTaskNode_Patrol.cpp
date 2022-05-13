#include "BehaviorTree/CBTTaskNode_Patrol.h"
#include "Global.h"

#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"

#include "Components/CPatrolComponent.h"


UCBTTaskNode_Patrol::UCBTTaskNode_Patrol()
{
	NodeName = "Patrol";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
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

	UCPatrolComponent* patrol = CHelpers::GetComponent<UCPatrolComponent>(ai);
	if (patrol == NULL)
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

	FVector location;
	float accept;

	if (patrol->GetMoveTo(location, accept) == false)
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

	UCPatrolComponent* patrol = CHelpers::GetComponent<UCPatrolComponent>(ai);
	if (patrol == NULL)
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

	FVector location;
	float acceptance;

	patrol->GetMoveTo(location, acceptance);
	EPathFollowingRequestResult::Type type = controller->MoveToLocation(location, acceptance, false);

	if (type == EPathFollowingRequestResult::Failed)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	if (type == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

		patrol->UpdateNextIndex();
	}
}

