#include "Characters/CAnimInstance.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CMontageComponent.h"
#include "Components/CParkourComponent.h"
#include "Components/CRidingComponent.h"
#include "Components/CStatusComponent.h"

#include "Characters/CHorse.h"

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	
	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(OwnerCharacter);

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	CheckNull(state);

	state->OnStateTypeChanged.AddDynamic(this, &UCAnimInstance::OnStateTypeChanged);
	state->OnMovementTypeChanged.AddDynamic(this, &UCAnimInstance::OnMovementTypeChanged);

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(OwnerCharacter);
	CheckNull(action);

	action->OnActionTypeChanged.AddDynamic(this, &UCAnimInstance::OnActionTypeChanged);
}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(OwnerCharacter);

	Speed = OwnerCharacter->GetVelocity().Size2D();
	Direction = CalculateDirection(OwnerCharacter->GetVelocity(), OwnerCharacter->GetControlRotation());

	InAir = OwnerCharacter->GetCharacterMovement()->IsFalling();

	UCParkourComponent* parkour = CHelpers::GetComponent<UCParkourComponent>(OwnerCharacter);
	CheckNull(parkour);
	MoveRight = parkour->GetMovingRight();
	CanMoveRight = parkour->GetCanMoveRight();
	MoveLeft = parkour->GetMovingLeft();
	CanMoveLeft = parkour->GetCanMoveLeft();
	Pitch = OwnerCharacter->GetBaseAimRotation().Pitch;

	UCFeetComponent* feet = CHelpers::GetComponent<UCFeetComponent>(OwnerCharacter);
	CheckNull(feet);

	if (feet != NULL)
	{
		FeetStop = feet->GetStop();
		Feet = feet->GetData();
	}
	
}

void UCAnimInstance::CanGrab(bool bCanGrabPI)
{
	Hanging = bCanGrabPI;
}

void UCAnimInstance::ClimbLedge(bool bIsClimbingPI)
{
	UCMontageComponent* montage = CHelpers::GetComponent<UCMontageComponent>(OwnerCharacter);
	CheckNull(montage);
	montage->PlayClimb();

	Hanging = bIsClimbingPI;
}

void UCAnimInstance::MoveLeftRight(float InAxis)
{
	if (InAxis > 0.0f)
	{
		MoveRight = true;
		MoveLeft = false;
	}

	if (InAxis < 0.0f)
	{
		MoveRight = false;
		MoveLeft = true;
	}
}

void UCAnimInstance::JumpLeft()
{
	UCMontageComponent* montage = CHelpers::GetComponent<UCMontageComponent>(OwnerCharacter);
	CheckNull(montage);
	OwnerCharacter->GetController()->DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	montage->PlayJumpLeft();
}

void UCAnimInstance::JumpRight()
{
	UCMontageComponent* montage = CHelpers::GetComponent<UCMontageComponent>(OwnerCharacter);
	CheckNull(montage);
	OwnerCharacter->GetController()->DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	montage->PlayJumpRight();
}

void UCAnimInstance::TurnLeft()
{
	UCMontageComponent* montage = CHelpers::GetComponent<UCMontageComponent>(OwnerCharacter);
	CheckNull(montage);
	montage->PlayTurnLeft();
}

void UCAnimInstance::TurnRight()
{
	UCMontageComponent* montage = CHelpers::GetComponent<UCMontageComponent>(OwnerCharacter);
	CheckNull(montage);
	montage->PlayTurnRight();
}

void UCAnimInstance::JumpUp()
{
	UCMontageComponent* montage = CHelpers::GetComponent<UCMontageComponent>(OwnerCharacter);
	CheckNull(montage);
	montage->PlayJumpUp();
}

void UCAnimInstance::TurnedBack(bool InTurnBack)
{
	TurnBack = InTurnBack;
}

void UCAnimInstance::GoUpWall()
{
	UCMontageComponent* montage = CHelpers::GetComponent<UCMontageComponent>(OwnerCharacter);
	CheckNull(montage);
	montage->PlayGoUpWall();
}

void UCAnimInstance::JumpOver()
{
	UCMontageComponent* montage = CHelpers::GetComponent<UCMontageComponent>(OwnerCharacter);
	CheckNull(montage);
	montage->PlayJumpOver();
}

void UCAnimInstance::WallRunRight()
{
	UCMontageComponent* montage = CHelpers::GetComponent<UCMontageComponent>(OwnerCharacter);
	CheckNull(montage);
	montage->PlayWallRunRight();
}

void UCAnimInstance::WallRunLeft()
{
	UCMontageComponent* montage = CHelpers::GetComponent<UCMontageComponent>(OwnerCharacter);
	CheckNull(montage);
	montage->PlayWallRunLeft();
}

void UCAnimInstance::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	StateType = InNewType;
}

void UCAnimInstance::OnMovementTypeChanged(EMovementType InPrevType, EMovementType InNewType)
{
	MovementType = InNewType;
}

void UCAnimInstance::OnActionTypeChanged(EActionType InPrevType, EActionType InNewType)
{
	ActionType = InNewType;
}
