#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "Components/TimelineComponent.h"
#include "CDoAction_GreatSword.generated.h"

UCLASS()
class MY_01_API ACDoAction_GreatSword : public ACDoAction
{
	GENERATED_BODY()

public:
	ACDoAction_GreatSword();

public:
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

	void SummonAction() override;
	void BeginSummonAction() override;
	void EndSummonAction() override;

	void SetSkillIndex(int32 InIndex) override;

	void CreateSkillEffect() override;

	void Reset() override;

	void HitGround();

private:
	UFUNCTION()
		void OnThrowBeginOverlap(FHitResult InHitResult, class ACharacter* InOtherCharacter);

	UFUNCTION()
		void OnGreatSwordSkillBeginOverlap(FHitResult InHitResult, class ACharacter* InOtherCharacter);

	UFUNCTION()
		void OnCannonBeginOverlap(FHitResult InHitResult, class ACharacter* InOtherCharacter);

	UFUNCTION()
		void OnCannonEndOverlap(class ACharacter* InOtherCharacter);

public:
	void OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) override;
	void OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) override;

private:
	void HitStopRestore() override;

private:
	void SetSpawnDecalLoaction();
	void SetSpawnSkillThrowLocation();

	void CreateSkillThrow();
	void CreateSkillDecal();

	void SpawnGreatSword();

	void DestroySkillEffects();

	void SwingGreatSword();

	void ReadData(const FDoActionData& InData, class ACharacter* InOtherCharacter);
	void ReadData(const FAirActionData& InData, class ACharacter* InOtherCharacter);
	void ReadData(const FSkillActionData& InData, class ACharacter* InOtherCharacter);
	void ReadData(const FSpecialActionData& InData, class ACharacter* InOtherCharacter);
	void ReadData(const FSummonActionData& InData, class ACharacter* InOtherCharacter);

private:
	UFUNCTION()
		void TunrGreatSword();

	UFUNCTION()
		void EndTunrGreatSword();

	UFUNCTION()
		void EndSwing();

	UFUNCTION()
		void Rolling_SpecialAction();

	UFUNCTION()
		void ExecutionFunction_SpecialAction();

private:

private:
	int32 ComboIndex = 0;
	bool bDoAction = true; 
	bool bBeginDoAction = true;

	int32 SkillIndex = 0;

	TArray<FVector> DecalLocations;
	TArray<FVector> SkillEffectLocations;

	TArray<class ACCreateDecal*> Decals;
	TArray<class ACSkillEffect*> SkillEffects;

	int32 AirComboIndex = 0;
	bool bAirAction = true;
	bool bBeginAirAction = true;

	class ACSkillEffect* GreatSword;
	float Radius = 480.0f;

	int32 SpecialActionIndex = 0;
	FVector TargetLocation;

	class ACCannon* Cannon;
	class ACCannonTarget* CannonTarget;
	class ACSummonController* SummonController;
	int32 SummonIndex = 0;

};
