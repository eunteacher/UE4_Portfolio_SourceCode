#include "Notifies/CAnimNotify_EndJumpUpLedge.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

#include "Components/CParkourComponent.h"

FString UCAnimNotify_EndJumpUpLedge::GetNotifyName_Implementation() const
{
	return "EndJumpUpLedge";
}


void UCAnimNotify_EndJumpUpLedge::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character);
	character->StopAnimMontage();

	UCParkourComponent* parkour = CHelpers::GetComponent<UCParkourComponent>(character);
	CheckNull(parkour);

	parkour->EndJumpUpLedge();

}


