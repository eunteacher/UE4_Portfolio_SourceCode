#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "CDoAction_Archer_Enemy.generated.h"

UCLASS()
class MY_01_API ACDoAction_Archer_Enemy : public ACDoAction
{
	GENERATED_BODY()

private:
	void BeginPlay() override;

public:
	void DoAction() override;
	void Begin_DoAction() override;
	void End_DoAction() override;

	void Reset() override;

public:
	void OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) override;

public:
	class ACFake* GetFakeArrow() { return FakeArrow; }

private:
	UFUNCTION()
		void OnThrowBeginOverlap(FHitResult InHitResult, class ACharacter* InOtherCharacter);

private:
	void HitStopRestore() override;

private:
	class ACFake* FakeArrow;
	class ACThrow* ThrowArrow;


};
