#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actions/CActionData.h"
#include "CDoAction.generated.h"

UCLASS()
class MY_01_API ACDoAction : public AActor
{
	GENERATED_BODY()
	
public: // 데이터 셋팅을 위한 함수
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
	// 기본 콤보 공격
	virtual void DoAction() {} // 시작
	virtual void Begin_DoAction() {} // 콤보 시작
	virtual void End_DoAction() {} // 콤보 끝
	// 공중 콤보 공격
	virtual void AirAction() {}
	virtual void Begin_AirAction() {}
	virtual void End_AirAction() {}
	// 스킬 공격
	virtual void SkillAction() {}
	virtual void Begin_SkillAction() {}
	virtual void End_SkillAction() {}
	// 스폐셜 공격
	virtual void SpecialAction() {}
	virtual void Begin_SpecialAction() {}
	virtual void End_SpecialAction() {}
	// 테이크 다운 공격
	virtual void TakedownAction() {}
	// 에임 온/오프
	virtual void OnAim() {}
	virtual void OffAim() {}

	// 막기 
	virtual void GuardAction() {}
	// 블록
	virtual void EndBlockAction() {}
	// 반격
	virtual void ParryAction() {}
	virtual void BeginParryAction() {}
	virtual void EndParryAction() {}
	// 소환
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
		class UCBehaviorCompoent* Behavior; // Player는 사용하지 말것

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

	bool bExist; // 다음 콤보 공격의 존재 유무
	bool bEnable; // 다음 콤보 공격을 사용 가능 유무
	bool IsLast; // 마지막 공격 유무

	bool CanTakedown;
	class ACharacter* VictimCharacter;

	bool bParry;
	class ACharacter* ParryTarget;

	float TimeLineRateTime;

	bool bReady; // ArcherSkill

	int32 ActionCount;

	bool bAirSwap; // 공중에서 무기 스왑이 가능 유무

};
