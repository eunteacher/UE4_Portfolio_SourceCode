#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CCannon.generated.h"

UCLASS()
class MY_01_API ACCannon : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category = "Target")
		class ACCannonTarget* Target;

public:
	FORCEINLINE class ACCannonTarget* GetTarget() { return Target; }
	
public:	
	ACCannon();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	void SetTarget(class ACCannonTarget* InTarget);

private:

};
