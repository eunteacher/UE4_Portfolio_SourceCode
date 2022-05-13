#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CBehaviorCompoent.generated.h"

UENUM(BlueprintType)
enum class EBehaviorType : uint8
{
	Wait, Approach, Action, Patrol, Hitted, Equip, Avoid, SkillAction, Guard, Dead, AirAction, SpecialAction, SummonAction, Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBehaviorTypeChange, EBehaviorType, InPrevType, EBehaviorType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MY_01_API UCBehaviorCompoent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		FName BehaviorKey = "Behavior";

	UPROPERTY(EditAnywhere)
		FName PlayerKey = "Player";

	UPROPERTY(EditAnywhere)
		FName AvoidLocationKey = "AvoidLocation";

	UPROPERTY(EditAnywhere)
		FName StartLocationKey = "StartLocation";

	UPROPERTY(EditAnywhere)
		FName EndLocationKey = "EndLocation";

public:
	FORCEINLINE void SetBlackboard(class UBlackboardComponent* InBlackboard) { Blackboard = InBlackboard; }
	
public:	
	UCBehaviorCompoent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintPure)
		bool IsWaitMode();

	UFUNCTION(BlueprintPure)
		bool IsApproachMode();

	UFUNCTION(BlueprintPure)
		bool IsActionMode();

	UFUNCTION(BlueprintPure)
		bool IsPatrolMode();

	UFUNCTION(BlueprintPure)
		bool IsHittedMode();

	UFUNCTION(BlueprintPure)
		bool IsEquipMode();

	UFUNCTION(BlueprintPure)
		bool IsAvoidMode();

	UFUNCTION(BlueprintPure)
		bool IsSkillActionMode();

	UFUNCTION(BlueprintPure)
		bool IsGuardMode();

	UFUNCTION(BlueprintPure)
		bool IsDeadMode();

	UFUNCTION(BlueprintPure)
		bool IsAirActionMode();

	UFUNCTION(BlueprintPure)
		bool IsSpecialActionMode();

	UFUNCTION(BlueprintPure)
		bool IsSummonActionMode();

public:
	void SetWaitMode();
	void SetAproachMode();
	void SetActionMode();
	void SetHittedMode();
	void SetEquipMode();
	void SetAvoidMode();
	void SetPatrolMode();
	void SetSkillActionMode();
	void SetGuardMode();
	void SetDeadMode();
	void SetAirActionMode();
	void SetSpecialActionMode();
	void SetSummonActionMode();

	class ACPlayer* GetTargetPlayer();
	FVector GetAvoidLocation();
	FVector GetStartLocation();
	FVector GetEndLocation();

	EBehaviorType GetBehaviorType();
private:
	void ChangeType(EBehaviorType InType);

public:
	// FBehaviorTypeChange, EBehaviorType, InPrevType, EBehaviorType, InNewType
	UPROPERTY(BlueprintAssignable)
		FBehaviorTypeChange OnBehaviorTypeChange;

private:
	class UBlackboardComponent* Blackboard;

public:
	FORCEINLINE bool GetBDice() { return bDice; }
	FORCEINLINE bool GetBDoAction() { return bDoAction; }
	FORCEINLINE bool GetBSkillAction() { return bSkillAction; }

	FORCEINLINE int32 GetRandomValue() { return RandomValue; }

	FORCEINLINE bool GetCanGuard() { return CanGuard; }
	
	void SetBDice(bool InBDice);
	void SetBDoAction(bool InBDoAction);
	void SetBSkillAction(bool InBSkillAction);

	void SetRandomValue(int32 InRandomValue);

	void SetCanGuard(bool InCanGuard);

	bool GetEndSkillAction() { return EndSkillAction; }
	void SetEndSkillAction(bool InEndSkillAction) { EndSkillAction = InEndSkillAction; }

private:
	bool bDice;
	bool bDoAction;
	bool bSkillAction;

	int32 RandomValue;

	bool CanGuard;

	bool EndSkillAction;

};
