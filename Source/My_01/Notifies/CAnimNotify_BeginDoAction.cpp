#include "CAnimNotify_BeginDoAction.h"
#include "Global.h"

#include "Actions/CActionData.h"
#include "Actions/CDoAction.h"
#include "Actions/CAttachment.h"

#include "Components/SkeletalMeshComponent.h"

#include "Components/CActionComponent.h"

#include "GameFramework/Character.h"

FString UCAnimNotify_BeginDoAction::GetNotifyName_Implementation() const
{
	return "Begin_DoAction";
}

void UCAnimNotify_BeginDoAction::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character);

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(character);
	CheckNull(action);

	if (action->GetCurrent()->GetDoAction() != NULL)
	{
		action->GetCurrent()->GetDoAction()->Begin_DoAction();
	}
}
