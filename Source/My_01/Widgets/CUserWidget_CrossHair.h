#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_CrossHair.generated.h"

UCLASS()
class MY_01_API UCUserWidget_CrossHair : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
		void SetAppear();

	UFUNCTION(BlueprintImplementableEvent)
		void SetDisappear();
	
	UFUNCTION(BlueprintImplementableEvent)
		void OnFocus();

	UFUNCTION(BlueprintImplementableEvent)
		void OffFocus();
	
};
