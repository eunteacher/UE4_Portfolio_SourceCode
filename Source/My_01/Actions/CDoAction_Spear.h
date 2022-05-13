#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "CDoAction_Spear.generated.h"

UCLASS()
class MY_01_API ACDoAction_Spear : public ACDoAction
{
	GENERATED_BODY()

public:
	void DoAction() override;
	void Begin_DoAction() override;
	void End_DoAction() override;

	void SkillAction() override;
	void Begin_SkillAction() override;
	void End_SkillAction() override;

	void TakedownAction() override;

	void SetOwnerLocation();
	void TakedonwSpawnHitEffect();

public:
	void OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) override;
	void OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) override;

private:
	void HitStopRestore() override;

	void ReadData(const FDoActionData& InData, class ACharacter* InOtherCharacter);
	void ReadData(const FAirActionData& InData, class ACharacter* InOtherCharacter);
	void ReadData(const FSkillActionData& InData, class ACharacter* InOtherCharacter);

	void Reset();

	UFUNCTION()
		void TakedownFinish();
	
private:
	int32 ComboIndex;
	int32 SkillIndex;
	int32 TakedownIndex;

};
