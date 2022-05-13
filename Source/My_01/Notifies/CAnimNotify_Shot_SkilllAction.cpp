#include "CAnimNotify_Shot_SkilllAction.h"
#include "Global.h"

#include "Actions/CActionData.h"
#include "Actions/CDoAction.h"
#include "Actions/CDoAction_Archer.h"

#include "Components/SkeletalMeshComponent.h"

#include "Components/CActionComponent.h"

#include "GameFramework/Character.h"

FString UCAnimNotify_Shot_SkilllAction::GetNotifyName_Implementation() const
{
	return "ShotArrow_Skill";
}

void UCAnimNotify_Shot_SkilllAction::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character);

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(character);
	CheckNull(action);

	ACDoAction_Archer* archer = Cast<ACDoAction_Archer>(action->GetCurrent()->GetDoAction());
	if (archer != NULL)
	{
		archer->ArcherSkill_SpawnShotEffect();
	}
	
}
