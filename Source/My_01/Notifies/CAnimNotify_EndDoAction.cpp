#include "CAnimNotify_EndDoAction.h"
#include "Global.h"

#include "Characters/CPlayer.h"

#include "Actions/CActionData.h"
#include "Actions/CDoAction.h"

#include "Components/SkeletalMeshComponent.h"

#include "Components/CActionComponent.h"

#include "GameFramework/Character.h"

FString UCAnimNotify_EndDoAction::GetNotifyName_Implementation() const
{
	return "End_DoAction";
}

void UCAnimNotify_EndDoAction::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
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
		action->GetCurrent()->GetDoAction()->End_DoAction();
	}

}
