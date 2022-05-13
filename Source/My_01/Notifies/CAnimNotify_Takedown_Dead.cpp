#include "Notifies/CAnimNotify_Takedown_Dead.h"
#include "CAnimNotify_Dead.h"
#include "Global.h"

#include "GameFramework/Character.h"

#include "Characters/ICharacter.h"

#include "Components/SkeletalMeshComponent.h"

FString UCAnimNotify_Takedown_Dead::GetNotifyName_Implementation() const
{
	return "Takedown_Dead";
}

void UCAnimNotify_Takedown_Dead::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character);

	IICharacter* character_i = Cast<IICharacter>(character);
	CheckNull(character);

	character_i->Takedown_Dead();

}
