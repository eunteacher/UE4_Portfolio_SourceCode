#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent.generated.h"

UENUM(BlueprintType)
enum class EStateType : uint8
{
	Idle, Parkour, Block, Parry, Grapple, Equip, Action, Hitted, Dead, Step, AirAction, SkillAction, TakedownAction, SpecialAction, Avoid, Riding, AimAndOverdraw, SummonAction, Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged, EStateType, InPrevType, EStateType, InNewType);

UENUM(BlueprintType)
enum class EMovementType : uint8
{
	Walking, Flying, Crouching, Running, Guarding, Riding, Sprinting, Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMovementTypeChanged, EMovementType, InPrevType, EMovementType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MY_01_API UCStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
		FORCEINLINE bool IsIdle() { return StateType == EStateType::Idle; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FORCEINLINE bool IsDead() { return StateType == EStateType::Dead; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FORCEINLINE bool IsParkour() { return StateType == EStateType::Parkour; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FORCEINLINE bool IsEquip() { return StateType == EStateType::Equip; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FORCEINLINE bool IsAction() { return StateType == EStateType::Action; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FORCEINLINE bool IsAirAction() { return StateType == EStateType::AirAction; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FORCEINLINE bool IsSkillAction() { return StateType == EStateType::SkillAction; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FORCEINLINE bool IsHitted() { return StateType == EStateType::Hitted; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FORCEINLINE bool IsStep() { return StateType == EStateType::Step; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FORCEINLINE bool IsGrapple() { return StateType == EStateType::Grapple; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FORCEINLINE bool IsTakedownAction() { return StateType == EStateType::TakedownAction; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FORCEINLINE bool IsSpecialAction() { return StateType == EStateType::SpecialAction; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FORCEINLINE bool IsBlock() { return StateType == EStateType::Block; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FORCEINLINE bool IsParry() { return StateType == EStateType::Parry; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
		FORCEINLINE bool IsAvoid() { return StateType == EStateType::Avoid; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FORCEINLINE bool IsRiding_State() { return StateType == EStateType::Riding; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FORCEINLINE bool IsAimAndOverdraw() { return StateType == EStateType::AimAndOverdraw; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FORCEINLINE bool IsSummonAction() { return StateType == EStateType::SummonAction; }

	///////////////////////////////////////////
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
		FORCEINLINE bool IsWalking() { return MovementType == EMovementType::Walking; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FORCEINLINE bool IsFlying() { return MovementType == EMovementType::Flying; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
		FORCEINLINE bool IsCrouching() { return MovementType == EMovementType::Crouching; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FORCEINLINE bool IsRunning() { return MovementType == EMovementType::Running; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FORCEINLINE bool IsGuarding() { return MovementType == EMovementType::Guarding; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FORCEINLINE bool IsRiding_Movement() { return MovementType == EMovementType::Riding; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FORCEINLINE bool IsSprinting() { return MovementType == EMovementType::Sprinting; }

	
public:
	FORCEINLINE EStateType GetStateType() { return StateType; }
	FORCEINLINE EMovementType GetMovementType() { return MovementType; }

public:	
	UCStateComponent();

protected:
	virtual void BeginPlay() override;

public: // Set State Mode
	void SetIdleMode();
	void SetParkourMode();
	void SetEquipMode();
	void SetActionMode();
	void SetSkillActionMode();
	void SetAirActionMode();
	void SetHittedMode();
	void SetStepMode();
	void SetDeadMode();
	void SetGrappleMode();
	void SetTakedownActionMode();
	void SetSpecialActionMode();
	void SetBlockMode();
	void SetParryMode();
	void SetAvoidMode();
	void SetRidingMode_State();
	void SetAimAndOverdrawMode();
	void SetSummonActionMode();

public:
	void SetWalkingMode();
	void SetFlyingMode();
	void SeCrouchingMode();
	void SetRunningMode();
	void SetGuardingMode();
	void SetRidingMode_Movement();
	void SetSprintingMode();

private:
	void ChangeStateType(EStateType InNewType);
	void ChangeMovementType(EMovementType InNewType);

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FStateTypeChanged OnStateTypeChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FMovementTypeChanged OnMovementTypeChanged;

private:
	EStateType StateType;
	EMovementType MovementType;
		
};
