#include "Notifies/CAnimNotify_EndAvoid.h"
#include "Global.h"

#include "Components/SkeletalMeshComponent.h"

#include "Components/CStateComponent.h"
#include "Components/CBehaviorCompoent.h"

#include "GameFramework/Character.h"

#include "Characters/CEnemy_AI_Archer.h"

FString UCAnimNotify_EndAvoid::GetNotifyName_Implementation() const
{
	return "EndAvoid";
}

void UCAnimNotify_EndAvoid::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character);

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(character);
	CheckNull(state);

	state->SetIdleMode();

	UCBehaviorCompoent* behavior = CHelpers::GetComponent<UCBehaviorCompoent>(character->GetController());
	CheckNull(behavior);

	FVector start = character->GetActorLocation();
	start.Z = 0.0f;
	
	ACEnemy_AI_Archer* enemy = Cast<ACEnemy_AI_Archer>(character);
	CheckNull(enemy);

	FVector target = enemy->GetPlayerLocation();
	target.Z = 0.0f;
	
	FRotator rotation = UKismetMathLibrary::FindLookAtRotation(start, target);

	character->SetActorRotation(rotation);
}

