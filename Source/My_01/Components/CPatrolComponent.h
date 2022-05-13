#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPatrolComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MY_01_API UCPatrolComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		class ACPatrolPath* Path;

	UPROPERTY(EditAnywhere)
		int32 Index;

	UPROPERTY(EditAnywhere)
		bool bReverse;

	UPROPERTY(EditAnywhere)
		float AcceptanceRadius = 50.0f;

public:
	FORCEINLINE bool IsValid() { return Path != NULL; }
	
public:	
	UCPatrolComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	bool GetMoveTo(FVector& OutLocation, float& OutAcceptanceRadius);
	void UpdateNextIndex();
		
};
