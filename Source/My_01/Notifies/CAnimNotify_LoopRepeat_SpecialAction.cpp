#include "CAnimNotify_LoopRepeat_SpecialAction.h"
#include "Global.h"

#include "Actions/CActionData.h"
#include "Actions/CDoAction.h"
#include "Actions/CDoAction_Dual.h"

#include "Components/SkeletalMeshComponent.h"

#include "Components/CActionComponent.h"

#include "GameFramework/Character.h"


FString UCAnimNotify_LoopRepeat_SpecialAction::GetNotifyName_Implementation() const
{
	return "LoopRepeat_SpecialAction";
}

void UCAnimNotify_LoopRepeat_SpecialAction::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
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
		dual->LoopRepeat_SpecialAction();
	}
	
	//action->GetCurrent()->GetDoAction()->Begin_SpecialAction();
}
