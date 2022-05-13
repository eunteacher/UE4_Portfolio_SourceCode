#include "Notifies/CAnimNotify_Dead.h"
#include "CAnimNotify_Dead.h"
#include "Global.h"

#include "Characters/ICharacter.h"

#include "Components/SkeletalMeshComponent.h"


FString UCAnimNotify_Dead::GetNotifyName_Implementation() const
{
	return "Dead";
}

void UCAnimNotify_Dead::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	IICharacter* character = Cast<IICharacter>(MeshComp->GetOwner());
	CheckNull(character);

	character->End_Dead();

}
