#include "CAnimNotify_StarLoop_SpecialAction.h"
#include "Global.h"

#include "GameFramework/Character.h"

#include "Actions/CActionData.h"
#include "Actions/CDoAction.h"
#include "Actions/CDoAction_Dual.h"

#include "Components/SkeletalMeshComponent.h"

#include "Components/CActionComponent.h"

FString UCAnimNotify_StarLoop_SpecialAction::GetNotifyName_Implementation() const
{
	return "StarLoop_SpecialAction";
}

void UCAnimNotify_StarLoop_SpecialAction::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character);

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(character);
	CheckNull(action);

	ACDoAction_Dual* dual = Cast<ACDoAction_Dual>(action->GetCurrent()->GetDoAction());
	if (dual != NULL)
	{
		dual->StarLoop_SpecialAction();
	}
}
