#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IParkour.generated.h"

UINTERFACE(MinimalAPI)
class UIParkour : public UInterface
{
	GENERATED_BODY()
};

class MY_01_API IIParkour
{
	GENERATED_BODY()

public:
	virtual void CanGrab(bool bCanGrabPI) = 0;
	virtual void ClimbLedge(bool bIsClimbingPI) = 0;
	virtual void MoveLeftRight(float InAxis) = 0;
	virtual void JumpLeft() = 0;
	virtual void JumpRight() = 0;
	virtual void TurnLeft() = 0;
	virtual void TurnRight() = 0;
	virtual void JumpUp() = 0;
	virtual void TurnedBack(bool InTurnBack) = 0;
	virtual void GoUpWall() = 0;
	virtual void JumpOver() = 0;
	virtual void WallRunRight() = 0;
	virtual void WallRunLeft() = 0;



};
