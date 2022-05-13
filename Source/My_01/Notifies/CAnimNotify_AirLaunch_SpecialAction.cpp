#include "CAnimNotify_AirLaunch_SpecialAction.h"
#include "Global.h"

#include "Actions/CActionData.h"
#include "Actions/CDoAction.h"
#include "Actions/CDoAction_Dual.h"

#include "Components/SkeletalMeshComponent.h"

#include "Components/CActionComponent.h"

#include "GameFramework/Character.h"

FString UCAnimNotify_AirLaunch_SpecialAction::GetNotifyName_Implementation() const
{
	return "AirLaunch_SpecialAction";
}

void UCAnimNotify_AirLaunch_SpecialAction::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
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
		dual->AirLaunch_SpecialAction();
	}
	
	//action->GetCurrent()->GetDoAction()->Begin_SpecialAction();
}
