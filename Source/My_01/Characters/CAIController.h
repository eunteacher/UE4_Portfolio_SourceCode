#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIController.generated.h"

UCLASS()
class MY_01_API ACAIController : public AAIController
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Behavior")
		float DoActionRange = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Behavior")
		float AirActionRange = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Behavior")
		float SkillActionRange = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Behavior")
		float SpecialActionRange = 700.0f;

	UPROPERTY(EditAnywhere, Category = "Behavior")
		float SummonActionRange = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Behavior")
		float AvoidRange = 500.0f;


	UPROPERTY(EditAnywhere, Category = "DrawDebug")
		bool bDrawDebug = false;

	UPROPERTY(EditAnywhere, Category = "DrawDebug")
		float AdjustCircleHeight = 50.0f;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UAIPerceptionComponent* Perception;

	UPROPERTY(VisibleDefaultsOnly)
		class UCBehaviorCompoent* Behavior;

public:
	FORCEINLINE float GetDoActionRange() { return DoActionRange; }
	FORCEINLINE float GetAirActionRange() { return AirActionRange; }
	FORCEINLINE float GetSkillActionRange() { return SkillActionRange; }
	FORCEINLINE float GetSpecialActionRange() { return SpecialActionRange; }
	FORCEINLINE float GetSummonActionRange() { return SummonActionRange; }
	FORCEINLINE float GetAvoidRange() { return AvoidRange; }

public:
	ACAIController();
	
public:
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

public:
	float GetSightRadius();

private:
	UFUNCTION()
		void OnPerceptionUpdated(const TArray<AActor*>& UpdataedActors);

private:
	class ACEnemy_AI* OwnerEnemy;
	class UAISenseConfig_Sight* Sight;
	
};
