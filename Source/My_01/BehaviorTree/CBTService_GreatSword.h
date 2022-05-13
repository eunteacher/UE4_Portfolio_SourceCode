#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_GreatSword.generated.h"

UCLASS()
class MY_01_API UCBTService_GreatSword : public UBTService
{
	GENERATED_BODY()

public:
	UCBTService_GreatSword();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool FindTarget;

	bool bFirstSummon;
	bool bSecondSummon;
	bool bThirdSummon;

	bool bDoAction;
	bool bSkillAction;
	bool bSpecialAction;
};
