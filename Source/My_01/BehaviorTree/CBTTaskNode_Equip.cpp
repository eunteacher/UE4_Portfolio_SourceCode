#include "BehaviorTree/CBTTaskNode_Equip.h"
#include "Global.h"

#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"

#include "Components/CActionComponent.h"

UCBTTaskNode_Equip::UCBTTaskNode_Equip()
{
	NodeName = "Equip";
}

EBTNodeResult::Type UCBTTaskNode_Equip::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	
	action->SetGreatSwordMode();

	if (action->IsUnarmedMode() == true)
	{
		return EBTNodeResult::Failed;
	}
	else
	{
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::InProgress;

}

