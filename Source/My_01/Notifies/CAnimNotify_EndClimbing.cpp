#include "Notifies/CAnimNotify_EndClimbing.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

#include "Components/CParkourComponent.h"

FString UCAnimNotify_EndClimbing::GetNotifyName_Implementation() const
{
	return "EndClimbing";
}


void UCAnimNotify_EndClimbing::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character);
	character->StopAnimMontage();

	UCParkourComponent* parkour = CHelpers::GetComponent<UCParkourComponent>(character);
	CheckNull(parkour);

	parkour->EndClimbLedge();

	//CLog::Log("Notify EndClimbing");
}


