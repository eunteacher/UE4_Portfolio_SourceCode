#include "Notifies/CAnimNotify_GrappleStart.h"
#include "global.h"

#include "GameFramework/Character.h"

#include "Components/SkeletalMeshComponent.h"

#include "Components/CGrappleComponent.h"

FString UCAnimNotify_GrappleStart::GetNotifyName_Implementation() const
{
	return "GrappleStart";
}

void UCAnimNotify_GrappleStart::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character);

	UCGrappleComponent* grapple = CHelpers::GetComponent<UCGrappleComponent>(character);
	CheckNull(grapple);

	grapple->AttemptGrapple();
}
