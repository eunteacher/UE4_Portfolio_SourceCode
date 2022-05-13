#include "BehaviorTree/CBTService_TwoHand.h"
#include "Global.h"

#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CEnemy_AI_Warrior.h"
#include "Characters/CPlayer.h"

#include "Components/CBehaviorCompoent.h"
#include "Components/CStateComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CPatrolComponent.h"
#include "Components/CStatusComponent.h"

#include "Actions/CAction.h"
#include "Actions/CDoAction.h"

UCBTService_TwoHand::UCBTService_TwoHand()
{
	NodeName = "TwoHand";

	FindTarget = false;
}

void UCBTService_TwoHand::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

	if (state->IsDead() == true)
	{
		return;
	}

	if (state->IsHitted() == true)
	{
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
