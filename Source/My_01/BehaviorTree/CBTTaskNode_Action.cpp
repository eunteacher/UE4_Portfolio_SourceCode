#include "BehaviorTree/CBTTaskNode_Action.h"
#include "Global.h"

#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CPlayer.h"

#include "Components/CActionComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CBehaviorCompoent.h"

#include "Actions/CAction.h"
#include "Actions/CDoAction.h"

UCBTTaskNode_Action::UCBTTaskNode_Action()
{
	NodeName = "DoAction";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Action::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(ai);
	if (action == NULL)
	{
		return EBTNodeResult::Failed;
	}

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	if (state == NULL)
	{
		return EBTNodeResult::Failed;
	}

	if (state->IsDead() == true || state->IsAction() == true || state->IsFlying() == true)
	{
		return EBTNodeResult::Failed;
	}
	
	if (behavior->GetTargetPlayer() == NULL)
	{
		return EBTNodeResult::Failed;
	}

	ACharacter* player = Cast<ACharacter>(behavior->GetTargetPlayer());
	if (player == NULL)
	{
		return EBTNodeResult::Failed;
	}

	// 공격하는 순간 플레이어 쳐다보기 
	FVector start = ai->GetActorLocation();
	start.Z = 0.0f;

	FVector target = player->GetActorLocation();
	target.Z = 0.0f;

	FRotator rotation = UKismetMathLibrary::FindLookAtRotation(start, target);
	ai->SetActorRotation(rotation);

	action->DoAction();

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Action::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

	if (state->IsDead() == true || state->IsHitted() == true)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	if (state->IsIdle() == true)
	{
		//CLog::Log("Succeeded");
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

