#include "CAnimNotifyState_EnableParry.h"
#include "Global.h"

#include "GameFramework/Character.h"

#include "Actions/CDoAction_Dual.h"
#include "Actions/CDoAction.h"

#include "Components/SkeletalMeshComponent.h"

#include "Components/CActionComponent.h"

#include "Characters/CPlayer.h"
#include "Widgets/CUserWidget_StatusBar.h"


FString UCAnimNotifyState_EnableParry::GetNotifyName_Implementation() const
{
	return "EnableParry";
}

void UCAnimNotifyState_EnableParry::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
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
		DoAction->EnableParry();
	}
	//CLog::Log("EnableCombo");

	ACPlayer* player = Cast<ACPlayer>(character);
	CheckNull(player);

	player->GetStatusBar()->OnParry();
}

void UCAnimNotifyState_EnableParry::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
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
		DoAction->DisableParry();
	}

	ACPlayer* player = Cast<ACPlayer>(character);
	CheckNull(player);

	player->GetStatusBar()->OffParry();
}
