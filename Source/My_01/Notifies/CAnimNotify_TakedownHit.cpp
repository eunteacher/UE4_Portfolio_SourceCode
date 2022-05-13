#include "CAnimNotify_TakedownHit.h"
#include "Global.h"

#include "GameFramework/Character.h"

#include "Actions/CDoAction.h"
#include "Actions/CDoAction_Spear.h"

#include "Components/SkeletalMeshComponent.h"

#include "Components/CActionComponent.h"

FString UCAnimNotify_TakedownHit::GetNotifyName_Implementation() const
{
	return "TakedownHit";
}

void UCAnimNotify_TakedownHit::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character);

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(character);
	CheckNull(action);

	//ACDoAction_Spear* Spear = Cast<ACDoAction_Spear>(action->GetCurrent()->GetDoAction());
	//Spear->TakedonwSpawnHitEffect();
}
