#include "CAnimNotifyState_Sequence.h"
#include "Global.h"

#include "Characters/CPlayer.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

#include "Components/CRidingComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CMontageComponent.h"
#include "Components/CActionComponent.h"


FString UCAnimNotifyState_Sequence::GetNotifyName_Implementation() const
{
	return "Sequence";
}

void UCAnimNotifyState_Sequence::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(player);
	CheckNull(action);

	if (action->IsUnarmedMode() == true)
	{
		player->OnSequence_Unarmed();
	}

	if (action->IsArcherMode() == true)
	{
		player->OnSequence_Archer();
	}

}

void UCAnimNotifyState_Sequence::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(player);
	CheckNull(action);

	if (action->IsUnarmedMode() == true)
	{
		player->OffSequence_Unarmed();
	}

	if (action->IsArcherMode() == true)
	{
		player->OffSequence_Archer();
	}


}
