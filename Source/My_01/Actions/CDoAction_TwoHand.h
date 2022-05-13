#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "CDoAction_TwoHand.generated.h"

UCLASS()
class MY_01_API ACDoAction_TwoHand : public ACDoAction
{
	GENERATED_BODY()

public:
	void DoAction() override;
	void Begin_DoAction() override;
	void End_DoAction() override;

	void Reset() override;

public:
	void OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) override;
	void OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) override;

private:
	void HitStopRestore() override;

private:
	int32 ComboIndex;

};
