#include "CAnimNotify_BeginParryAction.h"
#include "Global.h"

#include "Characters/CPlayer.h"

#include "Actions/CActionData.h"
#include "Actions/CDoAction.h"

#include "Components/SkeletalMeshComponent.h"

#include "Components/CActionComponent.h"

#include "GameFramework/Character.h"


FString UCAnimNotify_BeginParryAction::GetNotifyName_Implementation() const
{
	return "Begin_ParryAction";
}

void UCAnimNotify_BeginParryAction::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
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
		action->GetCurrent()->GetDoAction()->BeginParryAction();
	}

}
