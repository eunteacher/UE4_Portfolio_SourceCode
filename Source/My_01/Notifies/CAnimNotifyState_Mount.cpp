#include "CAnimNotifyState_Mount.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

#include "Components/CRidingComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CMontageComponent.h"

FString UCAnimNotifyState_Mount::GetNotifyName_Implementation() const
{
	return "Mount";
}

void UCAnimNotifyState_Mount::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character);

	UCRidingComponent* riding = CHelpers::GetComponent<UCRidingComponent>(character);
	CheckNull(riding);

	riding->MoveMount();

}

void UCAnimNotifyState_Mount::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character);

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(character);
	CheckNull(state);

	UCRidingComponent* riding = CHelpers::GetComponent<UCRidingComponent>(character);
	CheckNull(riding);

	//character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	
	state->SetIdleMode();
	state->SetRidingMode_Movement();

	riding->EndMount();

}
