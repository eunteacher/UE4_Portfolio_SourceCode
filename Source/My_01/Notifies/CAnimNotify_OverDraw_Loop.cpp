#include "Notifies/CAnimNotify_OverDraw_Loop.h"
#include "Global.h"

#include "Components/SkeletalMeshComponent.h"

#include "Components/CActionComponent.h"

#include "Actions/CActionData.h"
#include "Actions/CDoAction.h"
#include "Actions/CAttachment.h"
#include "Actions/CDoAction_Archer.h"

#include "GameFramework/Character.h"

FString UCAnimNotify_OverDraw_Loop::GetNotifyName_Implementation() const
{
	return "OverDraw_Loop";
}

void UCAnimNotify_OverDraw_Loop::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character);

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(character);
	CheckNull(action);

	if (action->GetCurrent()->GetAttachment() != NULL)
	{
		action->GetCurrent()->GetAttachment()->OnOverDraw_Loop();
		
		ACDoAction_Archer* archer = Cast<ACDoAction_Archer>(action->GetCurrent()->GetDoAction());

		if (archer != NULL)
		{
			archer->EnableFire();
		}
	}
}