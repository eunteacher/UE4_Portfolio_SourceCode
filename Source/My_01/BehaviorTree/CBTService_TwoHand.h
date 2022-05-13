#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_TwoHand.generated.h"

UCLASS()
class MY_01_API UCBTService_TwoHand : public UBTService
{
	GENERATED_BODY()

public:
	UCBTService_TwoHand();
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool FindTarget;
};
