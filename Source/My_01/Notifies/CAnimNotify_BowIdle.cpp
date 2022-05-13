#include "Notifies/CAnimNotify_BowIdle.h"
#include "Global.h"

#include "Components/SkeletalMeshComponent.h"

#include "Components/CActionComponent.h"

#include "Actions/CActionData.h"
#include "Actions/CDoAction.h"
#include "Actions/CAttachment.h"

#include "GameFramework/Character.h"

FString UCAnimNotify_BowIdle::GetNotifyName_Implementation() const
{
	return "Bow_Idle";
}

void UCAnimNotify_BowIdle::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
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
		action->GetCurrent()->GetAttachment()->OnBowIdle();
	}
}