#include "Notifies/CAnimNotify_EndLand.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CActionComponent.h"

#include "Actions/CActionData.h"
#include "Actions/CDoAction.h"

FString UCAnimNotify_EndLand::GetNotifyName_Implementation() const
{
	return "EndLand";
}


void UCAnimNotify_EndLand::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character);

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(character);
	CheckNull(state);

	UCStatusComponent* status = CHelpers::GetComponent<UCStatusComponent>(character);
	CheckNull(status);

	state->SetWalkingMode();

	if(state->IsSkillAction() == true)
	{
		UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(character);
		CheckNull(action);

		if (action->GetCurrent()->GetDoAction() != NULL)
		{
			action->GetCurrent()->GetDoAction()->End_DoAction();
		}
	}
	
}


