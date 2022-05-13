#pragma once

#include "CoreMinimal.h"
#include "Characters/CEnemy.h"
#include "CEnemy_AI.generated.h"

UCLASS()
class MY_01_API ACEnemy_AI : public ACEnemy
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		uint8 TeamID = 1;

protected:
	UPROPERTY(VisibleDefaultsOnly)
		class UCPatrolComponent* Patrol;

public:
	FORCEINLINE class UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }
	FORCEINLINE uint8 GetTeamID() { return TeamID; }

public:
	FORCEINLINE void SetAccumulatedDamage(float InAccumulatedDamage) { AccumulatedDamage = InAccumulatedDamage; }
	FORCEINLINE float GetAccumulatedDamage() { return AccumulatedDamage; }

public:
	ACEnemy_AI();

protected:
	float AccumulatedDamage;
	
};
