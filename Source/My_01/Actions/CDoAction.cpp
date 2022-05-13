#include "CDoAction.h"
#include "Global.h"

#include "GameFramework/Character.h"

#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CBehaviorCompoent.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

ACDoAction::ACDoAction()
{
	PrimaryActorTick.bCanEverTick = true;

	IsDoAction = true;
	IsAirAction = false;
	IsSkillAction = false;
}

void ACDoAction::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNull(OwnerCharacter);

	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	CheckNull(State);
	Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);
	CheckNull(Status);
	Action = CHelpers::GetComponent<UCActionComponent>(OwnerCharacter);
	CheckNull(Action);

	if (OwnerCharacter != UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		// Enemy
		Behavior = CHelpers::GetComponent<UCBehaviorCompoent>(OwnerCharacter->GetController());
		CheckNull(Behavior);
	}

	Super::BeginPlay();
}

// 플레이어를 바라보게 하는 함수 
void ACDoAction::LookAtPlayer()
{
	CheckNull(Behavior);

	if (Behavior->GetTargetPlayer() != NULL)
	{
		ACharacter* player = Cast<ACharacter>(Behavior->GetTargetPlayer());

		FVector start = OwnerCharacter->GetActorLocation();
		start.Z = 0.0f;

		FVector target = player->GetActorLocation();
		target.Z = 0.0f;

		FRotator rotation = UKismetMathLibrary::FindLookAtRotation(start, target);
		OwnerCharacter->SetActorRotation(rotation);
	}
}



