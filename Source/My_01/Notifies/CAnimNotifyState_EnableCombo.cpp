#include "CAnimNotifyState_EnableCombo.h"
#include "Global.h"

#include "GameFramework/Character.h"

#include "Actions/CDoAction_Dual.h"
#include "Actions/CDoAction.h"

#include "Components/SkeletalMeshComponent.h"

#include "Components/CActionComponent.h"

FString UCAnimNotifyState_EnableCombo::GetNotifyName_Implementation() const
{
	return "EnableCombo";
}

void UCAnimNotifyState_EnableCombo::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character);

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(character);
	CheckNull(action);

	ACDoAction* DoAction = Cast<ACDoAction>(action->GetCurrent()->GetDoAction());
	if (DoAction != NULL)
	{
		DoAction->EnableCombo();
	}
	//CLog::Log("EnableCombo");
}

void UCAnimNotifyState_EnableCombo::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());
	
	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character);

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(character);
	CheckNull(action);

	ACDoAction* DoAction = Cast<ACDoAction>(action->GetCurrent()->GetDoAction());
	if (DoAction != NULL)
	{
		DoAction->DisableCombo();
	}
}
