#include "CAnimNotifyState_SetLoaction.h"
#include "Global.h"

#include "Actions/CDoAction.h"
#include "Actions/CDoAction_Spear.h"

#include "Components/SkeletalMeshComponent.h"

#include "Components/CActionComponent.h"

FString UCAnimNotifyState_SetLoaction::GetNotifyName_Implementation() const
{
	return "SetLoaction";
}

void UCAnimNotifyState_SetLoaction::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	//CLog::Log("EnableCombo");
}

void UCAnimNotifyState_SetLoaction::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	ACDoAction_Spear* Spear = Cast<ACDoAction_Spear>(action->GetCurrent()->GetDoAction());
	Spear->SetOwnerLocation();
	//CLog::Log("EnableCombo");
}

void UCAnimNotifyState_SetLoaction::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

}
