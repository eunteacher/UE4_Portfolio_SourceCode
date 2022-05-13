#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "COptionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MY_01_API UCOptionComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly)
		float HorizontalLookRate = 45.0f;

	UPROPERTY(EditDefaultsOnly)
		float VerticalLookRate = 45.0f;

public:
	FORCEINLINE float GetHorizontalLookRate() { return HorizontalLookRate; }
	FORCEINLINE float GetVerticalLookRate() { return VerticalLookRate; }
	
public:	
	UCOptionComponent();

protected:
	virtual void BeginPlay() override;
		
};
