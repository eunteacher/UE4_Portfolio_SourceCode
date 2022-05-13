#include "Components/CBehaviorCompoent.h"
#include "Global.h"

#include "Characters/CPlayer.h"

#include "BehaviorTree/BlackboardComponent.h"

UCBehaviorCompoent::UCBehaviorCompoent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CanGuard = true;
}

void UCBehaviorCompoent::BeginPlay()
{
	Super::BeginPlay();
}

void UCBehaviorCompoent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

bool UCBehaviorCompoent::IsWaitMode()
{
	return GetBehaviorType() == EBehaviorType::Wait;
}

bool UCBehaviorCompoent::IsApproachMode()
{
	return GetBehaviorType() == EBehaviorType::Approach;
}

bool UCBehaviorCompoent::IsActionMode()
{
	return GetBehaviorType() == EBehaviorType::Action;
}

bool UCBehaviorCompoent::IsPatrolMode()
{
	return GetBehaviorType() == EBehaviorType::Patrol;
}

bool UCBehaviorCompoent::IsHittedMode()
{
	return GetBehaviorType() == EBehaviorType::Hitted;
}

bool UCBehaviorCompoent::IsEquipMode()
{
	return GetBehaviorType() == EBehaviorType::Equip;
}

bool UCBehaviorCompoent::IsAvoidMode()
{
	return GetBehaviorType() == EBehaviorType::Avoid;
}

bool UCBehaviorCompoent::IsSkillActionMode()
{
	return GetBehaviorType() == EBehaviorType::SkillAction;
}

bool UCBehaviorCompoent::IsGuardMode()
{
	return GetBehaviorType() == EBehaviorType::Guard;
}

bool UCBehaviorCompoent::IsDeadMode()
{
	return GetBehaviorType() == EBehaviorType::Dead;
}

bool UCBehaviorCompoent::IsAirActionMode()
{
	return GetBehaviorType() == EBehaviorType::AirAction;
}

bool UCBehaviorCompoent::IsSpecialActionMode()
{
	return GetBehaviorType() == EBehaviorType::SpecialAction;
}

bool UCBehaviorCompoent::IsSummonActionMode()
{
	return GetBehaviorType() == EBehaviorType::SummonAction;
}

///////////////////////////////////////////

void UCBehaviorCompoent::SetWaitMode()
{
	ChangeType(EBehaviorType::Wait);
}

void UCBehaviorCompoent::SetAproachMode()
{
	ChangeType(EBehaviorType::Approach);
}

void UCBehaviorCompoent::SetActionMode()
{
	ChangeType(EBehaviorType::Action);
}

void UCBehaviorCompoent::SetHittedMode()
{
	ChangeType(EBehaviorType::Hitted);
}

void UCBehaviorCompoent::SetEquipMode()
{
	ChangeType(EBehaviorType::Equip);
}

void UCBehaviorCompoent::SetAvoidMode()
{
	ChangeType(EBehaviorType::Avoid);
}

void UCBehaviorCompoent::SetPatrolMode()
{
	ChangeType(EBehaviorType::Patrol);
}
void UCBehaviorCompoent::SetSkillActionMode()
{
	ChangeType(EBehaviorType::SkillAction);
}

void UCBehaviorCompoent::SetGuardMode()
{
	ChangeType(EBehaviorType::Guard);
}

void UCBehaviorCompoent::SetDeadMode()
{
	ChangeType(EBehaviorType::Dead);
}

void UCBehaviorCompoent::SetAirActionMode()
{
	ChangeType(EBehaviorType::AirAction);
}

void UCBehaviorCompoent::SetSpecialActionMode()
{
	ChangeType(EBehaviorType::SpecialAction);
}

void UCBehaviorCompoent::SetSummonActionMode()
{
	ChangeType(EBehaviorType::SummonAction);
}

////////////////////

ACPlayer* UCBehaviorCompoent::GetTargetPlayer()
{
	return Cast<ACPlayer>(Blackboard->GetValueAsObject(PlayerKey));
}

FVector UCBehaviorCompoent::GetAvoidLocation()
{
	return Blackboard->GetValueAsVector(AvoidLocationKey);
}

FVector UCBehaviorCompoent::GetStartLocation()
{
	return Blackboard->GetValueAsVector(StartLocationKey);
}

FVector UCBehaviorCompoent::GetEndLocation()
{
	return Blackboard->GetValueAsVector(EndLocationKey);
}

void UCBehaviorCompoent::ChangeType(EBehaviorType InType)
{
	EBehaviorType type = GetBehaviorType();
	Blackboard->SetValueAsEnum(BehaviorKey, (uint8)InType);

	if (OnBehaviorTypeChange.IsBound())
	{
		OnBehaviorTypeChange.Broadcast(type, InType);
	}
}

EBehaviorType UCBehaviorCompoent::GetBehaviorType()
{
	return (EBehaviorType)Blackboard->GetValueAsEnum(BehaviorKey);
}

void UCBehaviorCompoent::SetBDice(bool InBDice)
{
	bDice = InBDice;
}

void UCBehaviorCompoent::SetBDoAction(bool InBDoAction)
{
	bDoAction = InBDoAction;
}

void UCBehaviorCompoent::SetBSkillAction(bool InBSkillAction)
{
	bSkillAction = InBSkillAction;
}

void UCBehaviorCompoent::SetRandomValue(int32 InRandomValue)
{
	RandomValue = InRandomValue;
}

void UCBehaviorCompoent::SetCanGuard(bool InCanGuard)
{
	CanGuard = InCanGuard;
}
