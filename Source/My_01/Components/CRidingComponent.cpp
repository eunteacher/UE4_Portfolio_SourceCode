#include "Components/CRidingComponent.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Components/CMontageComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"

#include "Components/CapsuleComponent.h"

#include "Characters/CHorse.h"
#include "Characters/CPlayer.h"

UCRidingComponent::UCRidingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCRidingComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	bPossibleRiding = false;
	IsMountRight = false;
}

void UCRidingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCRidingComponent::OnRiding()
{
	CheckFalse(bPossibleRiding);

	UCMontageComponent* montage = CHelpers::GetComponent<UCMontageComponent>(OwnerCharacter);
	CheckNull(montage);
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	CheckNull(state);

	state->SetRidingMode_State();

	if (IsMountRight == true)
	{
		montage->PlayMountRight();
	}
	else
	{
		montage->PlayMountLeft();
	}

	if (Horse != NULL)
	{
		Horse->GetCapsuleComponent()->SetCollisionProfileName("Horse");
	}
}

void UCRidingComponent::MoveMount()
{
	USpringArmComponent* springArm = CHelpers::GetComponent<USpringArmComponent>(OwnerCharacter);
	CheckNull(springArm);

	springArm->bDoCollisionTest = false;

	FLatentActionInfo info;
	info.CallbackTarget = this;
	info.UUID = GetUniqueID();
	info.ExecutionFunction = "StopMoveMount";
	info.Linkage = 0;

	UKismetSystemLibrary::MoveComponentTo
	(
		OwnerCharacter->GetCapsuleComponent(),
		MountLocation,
		MountRotation,
		false,
		false,
		1.1f,
		false,
		EMoveComponentAction::Move,
		info
	);
}

void UCRidingComponent::StopMoveMount()
{
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	OwnerCharacter->GetCharacterMovement()->Velocity = FVector(0.0f, 0.0f, 0.0f);
}

void UCRidingComponent::EndMount()
{
	OwnerCharacter->AttachToActor(Horse, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true), "MountPoint");
	OwnerCharacter->SetActorLocation(MountLocation);

	Horse->SetCanRiding(true);
	GetWorld()->GetFirstPlayerController()->Possess(Horse); // Horse¿¡ ºùÀÇ 

	if (Cast<ACPlayer>(OwnerCharacter)->GetStatusBar() != NULL)
	{
		Cast<ACPlayer>(OwnerCharacter)->GetStatusBar()->OffRiding();
	}
}

void UCRidingComponent::OffRiding()
{
	UCMontageComponent* montage = CHelpers::GetComponent<UCMontageComponent>(OwnerCharacter);
	CheckNull(montage);
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	CheckNull(state);

	state->SetRidingMode_State();

	OwnerCharacter->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepRelative, true));
	
	FVector location = OwnerCharacter->GetActorLocation();
	location.Z -= 70.0f;
	OwnerCharacter->SetActorLocation(location);

	montage->PlayDismountLeft();

}

void UCRidingComponent::MoveDismount()
{
	FVector location = Horse->GetDismountLocation()->GetComponentLocation();
	FRotator rotation = OwnerCharacter->GetActorRightVector().Rotation();

	location.Z += 60.0f;

	FLatentActionInfo info;
	info.CallbackTarget = this;
	info.UUID = GetUniqueID();
	info.ExecutionFunction = "StopMoveDismount";
	info.Linkage = 0;

	UKismetSystemLibrary::MoveComponentTo
	(
		OwnerCharacter->GetCapsuleComponent(),
		location,
		rotation,
		false,
		false,
		1.1f,
		false,
		EMoveComponentAction::Move,
		info
	);
}

void UCRidingComponent::StopMoveDismount()
{
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UCRidingComponent::EndDismount()
{
	//CLog::Log("EndDismount");
	Horse->SetCanRiding(false);
	GetWorld()->GetFirstPlayerController()->Possess(OwnerCharacter);

	USpringArmComponent* springArm = CHelpers::GetComponent<USpringArmComponent>(OwnerCharacter);
	CheckNull(springArm);

	springArm->bDoCollisionTest = true;

	if (Horse != NULL)
	{
		Horse->GetCapsuleComponent()->SetCollisionProfileName("Pawn");
	}
}

void UCRidingComponent::SetMountValue(FVector InMountLocation, FRotator InMountRotation)
{
	//CheckTrue(bPossibleRiding);

	MountLocation = InMountLocation;
	MountRotation = InMountRotation;
}

void UCRidingComponent::SetMountDirectionVec(FVector InMountRightVec, FVector InMountLeftVec)
{
	//CheckTrue(bPossibleRiding);

	MountRightVec = InMountRightVec;
	MountLeftVec = InMountLeftVec;
}

void UCRidingComponent::SetHorse(ACHorse* InHorse)
{
	Horse = InHorse;
}

void UCRidingComponent::SetOnRiding()
{
	bPossibleRiding = true;
}

void UCRidingComponent::SetOffRiding()
{
	bPossibleRiding = false;;
}

void UCRidingComponent::SetMountRight()
{
	IsMountRight = true;
}

void UCRidingComponent::SetMountLeft()
{
	IsMountRight = false;;
}

