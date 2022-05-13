#include "Notifies/CAnimNotify_MoveMount.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

#include "Components/CRidingComponent.h"

FString UCAnimNotify_MoveMount::GetNotifyName_Implementation() const
{
	return "MoveMount";
}

void UCAnimNotify_MoveMount::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character);

	UCRidingComponent* riding = CHelpers::GetComponent<UCRidingComponent>(character);
	CheckNull(riding);

	riding->MoveMount();
	//CLog::Log("Notify EndClimbing");
}


