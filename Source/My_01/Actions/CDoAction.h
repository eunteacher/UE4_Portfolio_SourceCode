#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actions/CActionData.h"
#include "CDoAction.generated.h"

UCLASS()
class MY_01_API ACDoAction : public AActor
{
	GENERATED_BODY()
	
public: // ������ ������ ���� �Լ�
	void SetDoActionDatas(TArray<FDoActionData> InDatas) { DoActionDatas = InDatas; }
	void SetAirActionDatas(TArray<FAirActionData> InDatas) { AirActionDatas = InDatas; }
	void SetSkillActionDatas(TArray<FSkillActionData> InDatas) { SkillActionDatas = InDatas; }
	void SetTakedownActionDatas(TArray<FTakedownActionData> InDatas) { TakedownActionDatas = InDatas; }
	void SetSpecialActionDatas(TArray<FSpecialActionData> InDatas) { SpecialActionDatas = InDatas; }
	void SetGuardAndParrayActionDatas(TArray<FGuardAndParrayActionData> InDatas) { GuardAndParrayActionDatas = InDatas; }
	void SetSummonActionDatas(TArray<FSummonActionData> InDatas) { SummonActionDatas = InDatas; }

public:
	void SetEquipped(const bool* InEquipped) { bEquipped = InEquipped; }

	void EnableCombo() { bEnable = true; }
	void DisableCombo() { bEnable = false; }
	void SetCanTakedonw(bool InCanTakedown) { CanTakedown = InCanTakedown; }
	void EnableParry() { bParry = true; }
	void DisableParry() { bParry = true; }
	bool GetReady() { return bReady; }
	int32 GetActionCount() { return ActionCount; }

public:	
	ACDoAction();

protected:
	virtual void BeginPlay() override;

public:
	// �⺻ �޺� ����
	virtual void DoAction() {} // ����
	virtual void Begin_DoAction() {} // �޺� ����
	virtual void End_DoAction() {} // �޺� ��
	// ���� �޺� ����
	virtual void AirAction() {}
	virtual void Begin_AirAction() {}
	virtual void End_AirAction() {}
	// ��ų ����
	virtual void SkillAction() {}
	virtual void Begin_SkillAction() {}
	virtual void End_SkillAction() {}
	// ����� ����
	virtual void SpecialAction() {}
	virtual void Begin_SpecialAction() {}
	virtual void End_SpecialAction() {}
	// ����ũ �ٿ� ����
	virtual void TakedownAction() {}
	// ���� ��/����
	virtual void OnAim() {}
	virtual void OffAim() {}

	// ���� 
	virtual void GuardAction() {}
	// ���
	virtual void EndBlockAction() {}
	// �ݰ�
	virtual void ParryAction() {}
	virtual void BeginParryAction() {}
	virtual void EndParryAction() {}
	// ��ȯ
	virtual void SummonAction() {}
	virtual void BeginSummonAction() {}
	virtual void EndSummonAction() {}

	virtual void SetSkillIndex(int32 InIndex) {}

	virtual void CreateSkillEffect() {}

	virtual void Reset() {}

protected:
	UFUNCTION()
		virtual void HitStopRestore() {}

	virtual void LookAtPlayer();

public:
	UFUNCTION()
		virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) {}
	UFUNCTION()
		virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) {}
	
protected:
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly)
		class UCStateComponent* State;

	UPROPERTY(BlueprintReadOnly)
		class UCStatusComponent* Status;

	UPROPERTY(BlueprintReadOnly)
		class UCActionComponent* Action;

	UPROPERTY(BlueprintReadOnly)
		class UCBehaviorCompoent* Behavior; // Player�� ������� ����

public:
	bool GetIsDoAction() { return IsDoAction; }
	bool GetIsAirAction() { return IsAirAction; }
	bool GetIsSkillAction() { return IsSkillAction; }
	bool GetIsSpecialAction() { return IsSpecialAction; }
	bool GetIsSummonAction() { return IsSummonAction; }

protected:
	const bool* bEquipped;

	bool IsDoAction;
	bool IsAirAction;
	bool IsSkillAction;
	bool IsSpecialAction;
	bool IsGuardAndParrayAction;
	bool IsSummonAction;

	TArray<FDoActionData> DoActionDatas;
	TArray<FAirActionData> AirActionDatas;
	TArray<FSkillActionData> SkillActionDatas;
	TArray<FTakedownActionData> TakedownActionDatas;
	TArray<FSpecialActionData> SpecialActionDatas;
	TArray<FGuardAndParrayActionData> GuardAndParrayActionDatas;
	TArray<FSummonActionData> SummonActionDatas;

	TArray<class ACharacter* > HitCharacters;

	bool bExist; // ���� �޺� ������ ���� ����
	bool bEnable; // ���� �޺� ������ ��� ���� ����
	bool IsLast; // ������ ���� ����

	bool CanTakedown;
	class ACharacter* VictimCharacter;

	bool bParry;
	class ACharacter* ParryTarget;

	float TimeLineRateTime;

	bool bReady; // ArcherSkill

	int32 ActionCount;

	bool bAirSwap; // ���߿��� ���� ������ ���� ����

};
