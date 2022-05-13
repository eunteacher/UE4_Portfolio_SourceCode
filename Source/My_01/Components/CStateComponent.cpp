#include "Components/CStateComponent.h"
#include "Global.h"

UCStateComponent::UCStateComponent()
{

}

void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();

	StateType = EStateType::Idle;
	MovementType = EMovementType::Walking;
}

/////////////////////////////////////////

void UCStateComponent::SetIdleMode()
{
	ChangeStateType(EStateType::Idle);
}

void UCStateComponent::SetParkourMode()
{
	ChangeStateType(EStateType::Parkour);
}

void UCStateComponent::SetEquipMode()
{
	ChangeStateType(EStateType::Equip);
}

void UCStateComponent::SetActionMode()
{
	ChangeStateType(EStateType::Action);
}

void UCStateComponent::SetSkillActionMode()
{
	ChangeStateType(EStateType::SkillAction);
}

void UCStateComponent::SetAirActionMode()
{
	ChangeStateType(EStateType::AirAction);
}

void UCStateComponent::SetHittedMode()
{
	ChangeStateType(EStateType::Hitted);
}

void UCStateComponent::SetStepMode()
{
	ChangeStateType(EStateType::Step);
}

void UCStateComponent::SetDeadMode()
{
	ChangeStateType(EStateType::Dead);
}

void UCStateComponent::SetGrappleMode()
{
	ChangeStateType(EStateType::Grapple);
}

void UCStateComponent::SetTakedownActionMode()
{
	ChangeStateType(EStateType::TakedownAction);
}

void UCStateComponent::SetSpecialActionMode()
{
	ChangeStateType(EStateType::SpecialAction);
}

void UCStateComponent::SetBlockMode()
{
	ChangeStateType(EStateType::Block);
}

void UCStateComponent::SetParryMode()
{
	ChangeStateType(EStateType::Parry);
}

void UCStateComponent::SetAvoidMode()
{
	ChangeStateType(EStateType::Avoid);
}

void UCStateComponent::SetRidingMode_State()
{
	ChangeStateType(EStateType::Riding);
}

void UCStateComponent::SetAimAndOverdrawMode()
{
	ChangeStateType(EStateType::AimAndOverdraw);
}

void UCStateComponent::SetSummonActionMode()
{
	ChangeStateType(EStateType::SummonAction);
}

/////////////////////////////////////////

void UCStateComponent::SetWalkingMode()
{
	ChangeMovementType(EMovementType::Walking);
}

void UCStateComponent::SetFlyingMode()
{
	ChangeMovementType(EMovementType::Flying);
}

void UCStateComponent::SeCrouchingMode()
{
	ChangeMovementType(EMovementType::Crouching);
}

void UCStateComponent::SetRunningMode()
{
	ChangeMovementType(EMovementType::Running);
}

void UCStateComponent::SetGuardingMode()
{
	ChangeMovementType(EMovementType::Guarding);
}

void UCStateComponent::SetRidingMode_Movement()
{
	//ChangeStateType(EStateType::Riding);
	ChangeMovementType(EMovementType::Riding);
}

void UCStateComponent::SetSprintingMode()
{
	//ChangeStateType(EStateType::Riding);
	ChangeMovementType(EMovementType::Sprinting);
}

/////////////////////////////////////////

void UCStateComponent::ChangeStateType(EStateType InNewType)
{
	EStateType prevType = StateType;
	StateType = InNewType;

	if (OnStateTypeChanged.IsBound())
	{
		OnStateTypeChanged.Broadcast(prevType, StateType);
	}
}

void UCStateComponent::ChangeMovementType(EMovementType InNewType)
{
	EMovementType prevType = MovementType;
	MovementType = InNewType;

	if (OnMovementTypeChanged.IsBound())
	{
		OnMovementTypeChanged.Broadcast(prevType, MovementType);
	}
}
