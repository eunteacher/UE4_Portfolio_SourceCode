#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "CDoAction_Unarmed.generated.h"

UCLASS()
class MY_01_API ACDoAction_Unarmed : public ACDoAction
{
	GENERATED_BODY()

public:

private:
	void TakedownAction() override;

	UFUNCTION()
		void TakedownFinish();
	
private:
	int32 Index;
};
