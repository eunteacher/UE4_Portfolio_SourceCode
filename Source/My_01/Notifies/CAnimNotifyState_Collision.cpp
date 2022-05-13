#include "CAnimNotifyState_Collision.h"
#include "Global.h"

#include "GameFramework/Character.h"

#include "Actions/CAction.h"
#include "Actions/CAttachment.h"

#include "Components/SkeletalMeshComponent.h"

#include "Components/CActionComponent.h"


FString UCAnimNotifyState_Collision::GetNotifyName_Implementation() const
{
	return "Collision";
}

void UCAnimNotifyState_Collision::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character);

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(character);
	CheckNull(action);

	ACAttachment* attachment = action->GetCurrent()->GetAttachment();
	CheckNull(attachment);

	attachment->OnCollision();
}

void UCAnimNotifyState_Collision::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character);

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(character);
	CheckNull(action);

	ACAttachment* attachment = action->GetCurrent()->GetAttachment();
	CheckNull(attachment);

	attachment->OffCollision();
}


