#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CActionComponent.generated.h"

UENUM(BlueprintType)
enum class EActionType : uint8
{
	Unarmed, Dual, GreatSword, Archer, Spear, Warrior, Archer_Enemy, TwoHand, Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActionTypeChanged, EActionType, InPrevType, EActionType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MY_01_API UCActionComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		class UCActionData* ActionDataAssets[(int32)EActionType::Max];

public:
	UFUNCTION(BlueprintPure)
		FORCEINLINE class UCAction* GetCurrent() { return ActionDatas[(int32)ActionType]; }

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
		FORCEINLINE bool IsUnarmedMode() { return ActionType == EActionType::Unarmed; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FORCEINLINE bool IsDualMode() { return ActionType == EActionType::Dual; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FORCEINLINE bool IsArcherMode() { return ActionType == EActionType::Archer; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FORCEINLINE bool IsGreatSwordMode() { return ActionType == EActionType::GreatSword; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FORCEINLINE bool IsSpearMode() { return ActionType == EActionType::Spear; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FORCEINLINE bool IsWarriorMode() { return ActionType == EActionType::Warrior; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FORCEINLINE bool IsArcherEnemyMode() { return ActionType == EActionType::Archer_Enemy; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FORCEINLINE bool IsTwoHandMode() { return ActionType == EActionType::TwoHand; }

public:
	FORCEINLINE EActionType GetActionType() { return ActionType; }

public:	
	UCActionComponent();

protected:
	virtual void BeginPlay() override;

public: // Set Action Mode
	UFUNCTION(BlueprintCallable)
		void SetUnarmedMode();

	UFUNCTION(BlueprintCallable)
		void SetDualMode();

	UFUNCTION(BlueprintCallable)
		void SetArcherMode();
	
	UFUNCTION(BlueprintCallable)
		void SetGreatSwordMode();

	UFUNCTION(BlueprintCallable)
		void SetSpearMode();

	UFUNCTION(BlueprintCallable)
		void SetWarriorMode();

	UFUNCTION(BlueprintCallable)
		void SetArcherEnemyMode();

	UFUNCTION(BlueprintCallable)
		void SetTwoHandMode();

public:
	void DoAction();
	void AirAction();
	void SkillAction();
	void TakedownAction();
	void SpecialAction();
	void Guard();
	void Parry();
	void SummonAction();

	void OnAim();
	void OffAim();

	void Dead();
	void End_Dead();

private:
	void SetActionMode(EActionType InType);
	void ChangeActionType(EActionType InNewType);

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FActionTypeChanged OnActionTypeChanged;

private:
	EActionType ActionType;
	class UCAction* ActionDatas[(int32)EActionType::Max];
};
