#include "BehaviorTree/CBTService_Warrior.h"
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
#include "Components/CMontageComponent.h"


UCBTService_Warrior::UCBTService_Warrior()
{
	NodeName = "Warrior";

	FindTarget = false;

}

void UCBTService_Warrior::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

	UCStatusComponent* status = CHelpers::GetComponent<UCStatusComponent>(ai);
	CheckNull(status);

	UCMontageComponent* montage = CHelpers::GetComponent<UCMontageComponent>(ai);
	CheckNull(montage);

	if (state->IsDead() == true || montage->GetDeadType() == EDeadType::Takedown)
	{
		behavior->SetDeadMode();
		return;
	}

	float value = status->GetMaxHealth() * 0.3f;
	if (status->GetHealth() < value && state->IsGuarding() == false && Cast<ACEnemy_AI_Warrior>(ai)->GetBlockCount() < 3)
	{
		state->SetGuardingMode();
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
