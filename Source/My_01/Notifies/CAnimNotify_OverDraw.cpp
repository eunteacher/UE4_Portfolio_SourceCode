#include "Notifies/CAnimNotify_OverDraw.h"
#include "Global.h"

#include "Components/SkeletalMeshComponent.h"

#include "Components/CActionComponent.h"

#include "Actions/CActionData.h"
#include "Actions/CDoAction.h"
#include "Actions/CAttachment.h"

#include "GameFramework/Character.h"

FString UCAnimNotify_OverDraw::GetNotifyName_Implementation() const
{
	return "OverDraw";
}

void UCAnimNotify_OverDraw::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
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
		action->GetCurrent()->GetAttachment()->OnOverDraw();
	}
}