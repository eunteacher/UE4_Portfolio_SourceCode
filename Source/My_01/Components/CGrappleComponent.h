#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CGrappleComponent.generated.h"

UENUM(BlueprintType)
enum class EGrappleStateType : uint8
{
	Retracted, Firing, NeraingTarget, OnTarget, Max
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MY_01_API UCGrappleComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Grapple")
		TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::ForOneFrame;

	UPROPERTY(EditAnywhere, Category = "Grapple")
		TEnumAsByte<EObjectTypeQuery> ObjectType = EObjectTypeQuery::ObjectTypeQuery7;

	UPROPERTY(EditAnywhere, Category = "Grapple")
		TEnumAsByte<ETraceTypeQuery> TraceType = ETraceTypeQuery::TraceTypeQuery4;
	
	UPROPERTY(EditAnywhere, Category = "Grapple")
		float MaxGrappleDistance = 1500.0f;

	UPROPERTY(EditAnywhere, Category = "Grapple")
		float HookToTargetDistance = 500.0f;

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class ACGrappleHook> GrappleHookClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class ACGrappleRope> GrappleRopeClass;


public:	
	UCGrappleComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OnGrapple();

private:
	void Retracted();
	void NeraingTarget();

	class AActor* FindBestTarget(TArray<class AActor*> InTargets);
	void SetCurrentTarget(class ACGrappleTarget* InGrappleTarget);

	FVector TargetDirection();
	FVector GetDestinationLocation();
	FRotator GetDestinationRotation();

public:
	void AttemptGrapple();
	void Firing();
	void OnTarget();

	UFUNCTION(BlueprintCallable)
		void EndGrapple();

private:
	class ACharacter* OwnerCharacter;

	EGrappleStateType GrappleTargetStateType;
	class ACGrappleTarget* CurrentGrappleTarget;
	class ACGrappleHook* GrappleHook;
	class ACGrappleRope* GrappleRope;

};
