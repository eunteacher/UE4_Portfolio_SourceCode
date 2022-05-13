#include "Notifies/CAnimNotify_GrappleFiring.h"
#include "global.h"

#include "GameFramework/Character.h"

#include "Components/SkeletalMeshComponent.h"

#include "Components/CGrappleComponent.h"

FString UCAnimNotify_GrappleFiring::GetNotifyName_Implementation() const
{
	return "GrappleFiring";
}

void UCAnimNotify_GrappleFiring::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character);

	UCGrappleComponent* grapple = CHelpers::GetComponent<UCGrappleComponent>(character);
	CheckNull(grapple);

	grapple->Firing();
}
