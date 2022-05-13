#include "BehaviorTree/CBTTaskNode_SkillAction.h"
#include "Global.h"

#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"

#include "Components/CActionComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CBehaviorCompoent.h"

#include "Actions/CAction.h"
#include "Actions/CDoAction.h"

UCBTTaskNode_SkillAction::UCBTTaskNode_SkillAction()
{
	NodeName = "SkillAction";

	bNotifyTick = true;

}

EBTNodeResult::Type UCBTTaskNode_SkillAction::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	if (state->IsDead() == true || state->IsFlying() == true || state->IsSkillAction() == true)
	{
		return EBTNodeResult::Failed;
	}

	//CLog::Log("TaskNode SkillAction");
	if (action->GetCurrent()->GetDoAction() != NULL)
	{
		//CLog::Print(SkillIndex);

		if (SelectSkill == false)
		{
			action->GetCurrent()->GetDoAction()->SetSkillIndex(SkillIndex % 3);
			SkillIndex++;
		}
		else
		{
			action->GetCurrent()->GetDoAction()->SetSkillIndex(SkillIndex);
		}
	}

	ACharacter* target = Cast<ACharacter>(behavior->GetTargetPlayer());
	if (target == NULL)
	{
		return EBTNodeResult::Failed;
	}

	FVector startLocation = ai->GetActorLocation();
	startLocation.Z = 0;
	FVector targetLoaction = target->GetActorLocation();
	targetLoaction.Z = 0;

	FRotator rotator = UKismetMathLibrary::FindLookAtRotation(startLocation, targetLoaction);
	ai->SetActorRotation(FQuat(rotator));

	action->SkillAction();

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_SkillAction::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
