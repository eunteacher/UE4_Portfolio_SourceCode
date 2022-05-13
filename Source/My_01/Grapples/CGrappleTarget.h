#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGrappleTarget.generated.h"

UCLASS()
class MY_01_API ACGrappleTarget : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly)
		class UBillboardComponent* Billboard;
	
	UPROPERTY(EditDefaultsOnly)
		class UWidgetComponent* Widget;

	UPROPERTY(EditDefaultsOnly)
		class USphereComponent* Sphere;

public:	
	ACGrappleTarget();

protected:
	virtual void BeginPlay() override;

public:	
	void SetActive(bool InActive);
};
