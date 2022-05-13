#include "Components/CActionComponent.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CStatusComponent.h"

#include "Actions/CAction.h"
#include "Actions/CActionData.h"
#include "Actions/CAttachment.h"
#include "Actions/CEquipment.h"
#include "Actions/CDoAction.h"

#include "Characters/ICharacter.h"

UCActionComponent::UCActionComponent()
{

}

void UCActionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	ACharacter* character = Cast<ACharacter>(GetOwner());
	CheckNull(character);

	for (int32 i = 0; i < (int32)EActionType::Max; i++)
	{
		if (ActionDataAssets[i] != NULL)
		{
			ActionDataAssets[i]->BeginPlay(character, &ActionDatas[i]);
		}
	}
}

// Setting Action Type

void UCActionComponent::SetUnarmedMode()
{
	ACEquipment* equipment = ActionDatas[(int32)ActionType]->GetEquipment();
	CheckNull(equipment);

	if (ActionDatas[(int32)ActionType] != NULL)
	{
		equipment->UnEquip();
	}

	ChangeActionType(EActionType::Unarmed);
}

void UCActionComponent::SetDualMode()
{
	SetActionMode(EActionType::Dual);
}

void UCActionComponent::SetArcherMode()
{
	SetActionMode(EActionType::Archer);
}

void UCActionComponent::SetGreatSwordMode()
{
	SetActionMode(EActionType::GreatSword);
}

void UCActionComponent::SetSpearMode()
{
	SetActionMode(EActionType::Spear);
}

void UCActionComponent::SetWarriorMode()
{
	SetActionMode(EActionType::Warrior);
}

void UCActionComponent::SetArcherEnemyMode()
{
	SetActionMode(EActionType::Archer_Enemy);
}

void UCActionComponent::SetTwoHandMode()
{
	SetActionMode(EActionType::TwoHand);
}

void UCActionComponent::SetActionMode(EActionType InType)
{
	if (ActionType == InType)
	{
		SetUnarmedMode();

		return;
	}
	else if (IsUnarmedMode() == false)
	{
		ACEquipment* equipment = ActionDatas[(int32)ActionType]->GetEquipment();
		CheckNull(equipment);

		equipment->UnEquip();
	}

	if (ActionDatas[(int32)InType] != NULL)
	{
		//CLog::Log("ActionData");
		ACEquipment* equipment = ActionDatas[(int32)InType]->GetEquipment();
		CheckNull(equipment);

		equipment->Equip();
	}

	ChangeActionType(InType);
}

void UCActionComponent::ChangeActionType(EActionType InNewType)
{
	EActionType prevType = ActionType;
	ActionType = InNewType;

	ACEquipment* equipment = ActionDatas[(int32)ActionType]->GetEquipment();
	CheckNull(equipment);

	if (OnActionTypeChanged.IsBound()) // 여기에 바인드되어있는게 있다면
	{
		OnActionTypeChanged.Broadcast(prevType, ActionType);
	}
}

void UCActionComponent::DoAction()
{
	CheckTrue(IsUnarmedMode());

	if (ActionDatas[(int32)ActionType] != NULL)
	{
		if (ActionDatas[(int32)ActionType]->GetDoAction() != NULL)
		{
			ACDoAction* action = ActionDatas[(int32)ActionType]->GetDoAction();
			if (action != NULL)
			{
				//CLog::Print("ActionComponent DoAction");
				action->DoAction();
			}
		}
	}
}

void UCActionComponent::AirAction()
{
	CheckTrue(IsUnarmedMode());

	if (ActionDatas[(int32)ActionType] != NULL)
	{
		ACDoAction* action = ActionDatas[(int32)ActionType]->GetDoAction();

		if (action != NULL)
		{
			//CLog::Print("ActionComponent DoAction");
			action->AirAction();
		}
	}
}

void UCActionComponent::SkillAction()
{
	CheckTrue(IsUnarmedMode());

	if (ActionDatas[(int32)ActionType] != NULL)
	{
		ACDoAction* action = ActionDatas[(int32)ActionType]->GetDoAction();

		if (action != NULL)
		{
			//CLog::Log("ActionComponent SkillAction");
			action->SkillAction();
		}
	}
}

void UCActionComponent::TakedownAction()
{
	if (ActionDatas[(int32)ActionType] != NULL)
	{
		ACDoAction* action = ActionDatas[(int32)ActionType]->GetDoAction();

		if (action != NULL)
		{
			//CLog::Log("ActionComponent SkillAction");
			action->TakedownAction();
		}
	}
}

void UCActionComponent::SpecialAction()
{
	if (ActionDatas[(int32)ActionType] != NULL)
	{
		ACDoAction* action = ActionDatas[(int32)ActionType]->GetDoAction();

		if (action != NULL)
		{
			action->SpecialAction();
		}
	}
}

void UCActionComponent::Guard()
{
	if (ActionDatas[(int32)ActionType] != NULL)
	{
		ACDoAction* action = ActionDatas[(int32)ActionType]->GetDoAction();

		if (action != NULL)
		{
			action->GuardAction();
		}
	}
}

void UCActionComponent::Parry()
{
	if (ActionDatas[(int32)ActionType] != NULL)
	{
		ACDoAction* action = ActionDatas[(int32)ActionType]->GetDoAction();

		if (action != NULL)
		{
			action->ParryAction();
		}
	}
}

void UCActionComponent::SummonAction()
{
	if (ActionDatas[(int32)ActionType] != NULL)
	{
		ACDoAction* action = ActionDatas[(int32)ActionType]->GetDoAction();

		if (action != NULL)
		{
			action->SummonAction();
		}
	}
}

void UCActionComponent::OnAim()
{
	CheckTrue(IsUnarmedMode());

	if (ActionDatas[(int32)ActionType] != NULL)
	{
		ACDoAction* action = ActionDatas[(int32)ActionType]->GetDoAction();

		if (action != NULL)
		{
			action->OnAim();
		}
	}
}

void UCActionComponent::OffAim()
{
	CheckTrue(IsUnarmedMode());

	if (ActionDatas[(int32)ActionType] != NULL)
	{
		ACDoAction* action = ActionDatas[(int32)ActionType]->GetDoAction();

		if (action != NULL)
		{
			action->OffAim();
		}
	}
}

void UCActionComponent::Dead()
{
	for (int32 i = 0; i < (int32)EActionType::Max; i++)
	{
		if (ActionDatas[i] != NULL && ActionDatas[i]->GetAttachment() != NULL)
		{
			ActionDatas[i]->GetAttachment()->OffCollision();
		}
	}
}

void UCActionComponent::End_Dead()
{
	for (int32 i = 0; i < (int32)EActionType::Max; i++)
	{
		if (ActionDatas[i] != NULL && ActionDatas[i]->GetAttachment() != NULL)
		{
			ActionDatas[i]->GetAttachment()->Destroy();
		}
	}
}


