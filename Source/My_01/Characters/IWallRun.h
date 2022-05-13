#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IWallRun.generated.h"

UINTERFACE(MinimalAPI)
class UIWallRun : public UInterface
{
	GENERATED_BODY()
};

class MY_01_API IIWallRun
{
	GENERATED_BODY()

public:
	virtual void SetRunnableWall(bool Enable, bool IsRight, FVector Target, FTransform Start) = 0;
};
