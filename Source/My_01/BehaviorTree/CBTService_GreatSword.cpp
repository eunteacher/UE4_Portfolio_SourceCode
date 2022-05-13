#include "BehaviorTree/CBTService_GreatSword.h"
#include "Global.h"

#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CPlayer.h"
#include "Characters/CEnemy_AI_GirlKnight.h"

#include "Components/CBehaviorCompoent.h"
#include "Components/CStateComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CPatrolComponent.h"
#include "Components/CStatusComponent.h"

#include "Actions/CDoAction.h"
#include "Actions/CDoAction_GreatSword.h"

UCBTService_GreatSword::UCBTService_GreatSword()
{
	NodeName = "GreatSword";

	FindTarget = false;
	
	bFirstSummon = false;
	bSecondSummon = false;
	bThirdSummon = false;

	bDoAction = true;
	bSkillAction = false;
	bSpecialAction = false;

}

void UCBTService_GreatSword::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

	UCStatusComponent* status = CHelpers::GetComponent<UCStatusComponent>(ai);
	CheckNull(status);

	if (state->IsDead() == true)
	{
		behavior->SetDeadMode();
		return;
	}

	if (behavior->IsDeadMode() == true)
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
		if (FindTarget == false)
		{
			ai->FindTarget();
			FindTarget = true;
		}

		if (state->IsIdle() == false)
		{
			return;
		}

		// Summon
		float maxHealth = status->GetMaxHealth();
		float currentHealth = status->GetHealth();
		float AccumulatedDamage = ai->GetAccumulatedDamage();

		if (maxHealth * 0.8f > currentHealth && bFirstSummon == false)
		{
			behavior->SetSummonActionMode();
			bFirstSummon = true;
			return;
		}

		if (maxHealth * 0.5f > currentHealth && bSecondSummon == false)
		{
			behavior->SetSummonActionMode();
			bSecondSummon = true;
			return;
		}

		if (maxHealth * 0.3f > currentHealth && bThirdSummon == false)
		{
			behavior->SetSummonActionMode();
			bThirdSummon = true;
			return;
		}

		if (AccumulatedDamage > maxHealth * 0.1f && bSkillAction == false && bSpecialAction == false)
		{
			bDoAction = false;
			bSkillAction = true;
			bSpecialAction = true;

			ai->SetAccumulatedDamage(0.0f);
		}
		
		if (bSkillAction == true && bSpecialAction == true)
		{
			behavior->SetSkillActionMode();
			bSkillAction = false;
			return;
		}

		if (bSpecialAction == true && bSkillAction == false)
		{
			bSpecialAction = false;
			return;
		}

		// DoAction, AirAction, SkillAction, SpecialAction
		float approach = controller->GetSightRadius();
		float distance = ai->GetDistanceTo(target);
		float doAction = controller->GetDoActionRange();

		if (bSpecialAction == false && bSkillAction == false)
		{
			bDoAction = true;
		}

		if (distance < doAction && bDoAction == true)
		{
			if (UKismetMathLibrary::RandomBool() == true)
			{
				behavior->SetActionMode();
			}
			else
			{
				behavior->SetAirActionMode();
			}
			return;
		}

		if (distance < approach && bDoAction == true)
		{
			behavior->SetAproachMode();
			return;
		}
	}
}

