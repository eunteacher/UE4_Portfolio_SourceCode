#include "BehaviorTree/CBTService_Archer_Enemy.h"
#include "Global.h"

#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CPlayer.h"

#include "Components/CBehaviorCompoent.h"
#include "Components/CStateComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CPatrolComponent.h"
#include "Components/CMontageComponent.h"


UCBTService_Archer_Enemy::UCBTService_Archer_Enemy()
{
	NodeName = "Archer_Enemy";

	FindTarget = false;
}

void UCBTService_Archer_Enemy::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNull(controller);
	
	UCBehaviorCompoent* behavior = CHelpers::GetComponent<UCBehaviorCompoent>(controller);
	CheckNull(behavior);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNull(ai);

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	CheckNull(state);

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(ai);
	CheckNull(action);

	UCPatrolComponent* patrol = CHelpers::GetComponent<UCPatrolComponent>(ai);
	CheckNull(patrol);

	UCMontageComponent* montage = CHelpers::GetComponent<UCMontageComponent>(ai);
	CheckNull(montage);

	if (state->IsDead() == true || montage->GetDeadType() == EDeadType::Takedown)
	{
		behavior->SetDeadMode();
		return;
	}

	if (state->IsHitted() == true)
	{
		//CLog::Print("Hitted");
		behavior->SetHittedMode();
		return;
	}

	if (state->IsFlying() == true)
	{
		return;
	}

	ACPlayer* target = behavior->GetTargetPlayer();
	if (target != NULL)
	{
		ai->FindTarget();

		float approach = controller->GetSightRadius();
		float distance = ai->GetDistanceTo(target);
		float doAction = controller->GetDoActionRange();
		float avoid = controller->GetAvoidRange();

		if (distance < avoid)
		{
			behavior->SetAvoidMode();
			return;
		}

		if (distance < doAction)
		{
			behavior->SetActionMode();
			return;
		}

		if (distance < approach)
		{
			behavior->SetAproachMode();
			return;
		}
	}
	else
	{
		if (patrol->IsValid())
		{
			behavior->SetPatrolMode();
			return;
		}

	}
}
