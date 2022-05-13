#include "Notifies/CAnimNotify_MoveDismount.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

#include "Components/CRidingComponent.h"

FString UCAnimNotify_MoveDismount::GetNotifyName_Implementation() const
{
	return "MoveDismount";
}

void UCAnimNotify_MoveDismount::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character);

	UCRidingComponent* riding = CHelpers::GetComponent<UCRidingComponent>(character);
	CheckNull(riding);

	riding->MoveDismount();
	//CLog::Log("Notify EndClimbing");
}


