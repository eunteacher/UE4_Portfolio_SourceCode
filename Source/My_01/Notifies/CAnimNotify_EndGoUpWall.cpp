#include "Notifies/CAnimNotify_EndGoUpWall.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CStateComponent.h"

FString UCAnimNotify_EndGoUpWall::GetNotifyName_Implementation() const
{
	return "EndGoUpWall";
}

void UCAnimNotify_EndGoUpWall::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character);
	character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(character);
	CheckNull(state);

	state->SetIdleMode();
}


