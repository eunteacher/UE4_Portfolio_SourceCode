#include "Notifies/CAnimNotify_EndJumpLeftRight.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

#include "Components/CParkourComponent.h"

FString UCAnimNotify_EndJumpLeftRight::GetNotifyName_Implementation() const
{
	return "EndJumpLeftRight";
}


void UCAnimNotify_EndJumpLeftRight::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character);
	character->StopAnimMontage();

	UCParkourComponent* parkour = CHelpers::GetComponent<UCParkourComponent>(character);
	CheckNull(parkour);

	parkour->EndJumpLeftRight();

}


