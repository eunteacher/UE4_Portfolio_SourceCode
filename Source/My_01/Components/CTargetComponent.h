#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CTargetComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MY_01_API UCTargetComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		float TraceRadius = 1500.0f;

	UPROPERTY(EditAnywhere)
		float InteropSpeed = 2.5f;

	UPROPERTY(EditAnywhere, Category = "Target")
		TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::ForOneFrame;

public:
	class ACharacter* GetTarget() { return Target; }

public:	
	UCTargetComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void OnTarget();
	
private:
	void EndTargeting();
	void StartTargeting();

	void TraceTargets();
	void SetTarget();

	void ChangeTarget(class ACharacter* InTarget);
	
private:
	class ACharacter* OwnerCharacter;
	class ACharacter* Target;
	class UDecalComponent* TargetDecal;

	TArray<class ACharacter*> Targets;

};
