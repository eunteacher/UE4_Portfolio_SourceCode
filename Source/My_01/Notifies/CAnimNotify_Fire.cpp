#include "Notifies/CAnimNotify_Fire.h"
#include "Global.h"

#include "Components/SkeletalMeshComponent.h"

#include "Components/CActionComponent.h"

#include "Actions/CActionData.h"
#include "Actions/CDoAction.h"
#include "Actions/CAttachment.h"

#include "GameFramework/Character.h"

FString UCAnimNotify_Fire::GetNotifyName_Implementation() const
{
	return "Fire";
}

void UCAnimNotify_Fire::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
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
		action->GetCurrent()->GetAttachment()->OnFire();
	}
}