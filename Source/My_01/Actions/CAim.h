#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Widgets/CUserWidget_CrossHair.h"
#include "Components/TimelineComponent.h"
#include "CAim.generated.h"

UCLASS()
class MY_01_API UCAim : public UObject
{
	GENERATED_BODY()

public:
	bool IsAvaliable() { return SpringArm != NULL && Camera != NULL; }
	bool GetInZoom() { return bInZoom; }
	
public:
	UCAim();

	void BeginPlay(class ACharacter* InCharacter);

	void OnAim(); // 에임을 키는 함수
	void OffAim(); // 에임을 종료하는 함수 

	// 적 감지
	void ForwardTrace();

private:
	class ACharacter* OwnerCharacter;

	class USpringArmComponent* SpringArm;
	class UCameraComponent* Camera;

	bool bInZoom; // 에임 상태

	TSubclassOf<class UCUserWidget_CrossHair> CrossHairClass;
	class UCUserWidget_CrossHair* CrossHair;

	class UCStateComponent* State;

	bool bWithInRange;

	class UCAnimInstance* AnimInstatnce;
};
