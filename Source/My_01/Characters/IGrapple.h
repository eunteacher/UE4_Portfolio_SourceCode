#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IGrapple.generated.h"

UENUM(BlueprintType)
enum class EGrappleTargetStateType : uint8
{
	Using, NoUsing, Max
};


UINTERFACE(MinimalAPI)
class UIGrapple : public UInterface
{
	GENERATED_BODY()
};

class MY_01_API IIGrapple
{
	GENERATED_BODY()

public:
	virtual EGrappleTargetStateType GetGrappleTargetStateType() = 0;
};
