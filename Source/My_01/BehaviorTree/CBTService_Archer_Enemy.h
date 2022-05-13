#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Archer_Enemy.generated.h"

UCLASS()
class MY_01_API UCBTService_Archer_Enemy : public UBTService
{
	GENERATED_BODY()

public:
	UCBTService_Archer_Enemy();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool FindTarget;

	
};
