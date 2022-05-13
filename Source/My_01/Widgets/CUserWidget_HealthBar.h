#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_HealthBar.generated.h"

UCLASS()
class MY_01_API UCUserWidget_HealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
		void Update(float Health, float MaxHealth);
	
};
