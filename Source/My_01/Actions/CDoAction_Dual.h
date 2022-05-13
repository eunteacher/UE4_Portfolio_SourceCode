#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CDoAction_Dual.generated.h"

UCLASS()
class MY_01_API ACDoAction_Dual : public ACDoAction
{
	GENERATED_BODY()

public: // Action 함수
	void DoAction() override;
	void Begin_DoAction() override;
	void End_DoAction() override;

	void AirAction() override;
	void Begin_AirAction() override;
	void End_AirAction() override;

	void SkillAction() override;
	void Begin_SkillAction() override;
	void End_SkillAction() override;

	void SpecialAction() override;
	void Begin_SpecialAction() override;
	void End_SpecialAction() override;

	void GuardAction() override;

	void EndBlockAction() override;

	void ParryAction() override;
	void BeginParryAction() override;
	void EndParryAction() override;

public:
	void Turn_SpecialAction();
	void ComeBack_SpecialAction();
	void Loop_SpecialAction();
	void LoopRepeat_SpecialAction();

	void StarLoop_SpecialAction();

	void AirLaunch_SpecialAction();

	void TargetTakeDamage(int32 InIndex);

	void AirStar_Loop_SpecialAction();
	void AirStar_Last_SpecialAction();

private:
	void EndStar_SpecialAction();
	void AirStar_End_SpecialAction();

public:
	void OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) override;
	void OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) override;

	void Reset() override;

private:
	void HitStopRestore() override;

	UFUNCTION()
		void AirLaunch();

	UFUNCTION()
		void ExecutionFunction_SkillAction();

	UFUNCTION()
		void ExecutionFunction_SpecialAction();

	// 데이터 읽기
	void ReadData(const FDoActionData& InData, class ACharacter* InOtherCharacter);
	void ReadData(const FAirActionData& InData, class ACharacter* InOtherCharacter);
	void ReadData(const FSkillActionData& InData, class ACharacter* InOtherCharacter);
	void ReadData(const FGuardAndParrayActionData& InData, class ACharacter* InOtherCharacter);

	// SpecialEffect
	void SpawnSpecialEffect(FVector InStartLocation, FRotator InStartRotation, FVector InTargetLocation, int32 InIndex);
	// NiagaraEffect
	void SpawnNiagaraEffect(FVector InStartLocation, FRotator InStartRotation, int32 InIndex);

	void BlockAction();
	
private:
	int32 ComboIndex;
	int32 AirComboIndex;
	int32 SkillIndex;

	FVector StartUpVec;

	FVector SpecialAction_StartLocation;
	FRotator SpecialAction_StartRotation;
	FVector SpecialAction_StartForwardVec;

	bool bLoop;
	int32 SpecialActionMaxCount;
	int32 SpecialActionCount;

	bool bStar;
	int32 StarCount;

	bool bAirStar;
	bool bAirLaunch = false;
	
	TArray<class ACSpecialEffect*> Effects;

	FVector SpecialAction_EndLocation;
	FVector SpecialAction_EndForwardVec;

	float TargetDistance;

	class ACharacter* Target;
	TArray<class ACharacter*> Targets;

	FVector CenterPoint;
	FVector AirCenterPoint;
};



