#pragma once

#include "CoreMinimal.h"
#include "CableActor.h"
#include "CGrappleRope.generated.h"

UCLASS()
class MY_01_API ACGrappleRope : public ACableActor
{
	GENERATED_BODY()

public:
	ACGrappleRope();

	UFUNCTION(BlueprintImplementableEvent)
		void OnDestroy();
};
