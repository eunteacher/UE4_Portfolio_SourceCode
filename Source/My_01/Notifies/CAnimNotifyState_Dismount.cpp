#include "CAnimNotifyState_Dismount.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

#include "Components/CRidingComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CMontageComponent.h"


FString UCAnimNotifyState_Dismount::GetNotifyName_Implementation() const
{
	return "Dismount";
}

void UCAnimNotifyState_Dismount::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character);

	UCRidingComponent* riding = CHelpers::GetComponent<UCRidingComponent>(character);
	CheckNull(riding);

	riding->MoveDismount();
}

void UCAnimNotifyState_Dismount::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character);

	UCRidingComponent* riding = CHelpers::GetComponent<UCRidingComponent>(character);
	CheckNull(riding);

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(character);
	CheckNull(state);

	state->SetIdleMode();
	state->SetWalkingMode();

	riding->EndDismount();

}
