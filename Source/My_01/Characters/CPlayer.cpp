#include "CPlayer.h"
#include "Global.h"

#include "Camera/CameraComponent.h"

#include "Animation/AnimInstance.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Components/COptionComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CFeetComponent.h"
#include "Components/CParkourComponent.h"
#include "Components/CMontageComponent.h"
#include "Components/CGrappleComponent.h"
#include "Components/CTargetComponent.h"
#include "Components/CRidingComponent.h"

#include "Components/CapsuleComponent.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

#include "Actions/CAction.h"
#include "Actions/CActionData.h"
#include "Actions/CDoAction.h"

#include "Characters/CEnemy_AI_GirlKnight.h"

#include "C:\Program Files\Epic Games\UE_4.26\Engine\Plugins\Runtime\CableComponent\Source\CableComponent\Classes/CableComponent.h"

ACPlayer::ACPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetMesh());

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

	// Create Components
	Option = CreateDefaultSubobject<UCOptionComponent>("Option");
	Status = CreateDefaultSubobject<UCStatusComponent>("Status");
	State = CreateDefaultSubobject<UCStateComponent>("State");
	Action = CreateDefaultSubobject<UCActionComponent>("Action");
	Feet = CreateDefaultSubobject<UCFeetComponent>("Feet");
	Parkour = CreateDefaultSubobject<UCParkourComponent>("Parkour");
	Montage = CreateDefaultSubobject<UCMontageComponent>("Montage");
	Grapple = CreateDefaultSubobject<UCGrappleComponent>("Grapple");
	Target = CreateDefaultSubobject<UCTargetComponent>("Target");
	Riding = CreateDefaultSubobject<UCRidingComponent>("Riding");

	bUseControllerRotationYaw = false;

	// Mesh 기본값 설정
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/GhostLady_S1/Meshes/Characters/Combines/SK_AssassinA.SK_AssassinA'");
	GetMesh()->SetSkeletalMesh(mesh);


	// AnimBlueprint'/Game/Player/ABP_CPlayer.ABP_CPlayer'
	TSubclassOf<UAnimInstance> animInstance;
	CHelpers::GetClass<UAnimInstance>(&animInstance, "AnimBlueprint'/Game/Player/ABP_CPlayer.ABP_CPlayer_C'");
	GetMesh()->SetAnimInstanceClass(animInstance);

	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 140.0f));
	SpringArm->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	SpringArm->TargetArmLength = 500.0f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bDoCollisionTest = true;

	Camera->SetRelativeLocation(FVector(0.0f, 0.0f, 140.0f));
	Camera->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));

	GetCharacterMovement()->MaxWalkSpeed = Status->GetRunSpeed();
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->JumpZVelocity = 450.0f;
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();
	CheckNull(Action);
	CheckNull(State);

	State->OnStateTypeChanged.AddDynamic(this, &ACPlayer::OnStateTypeChanged);
	State->OnMovementTypeChanged.AddDynamic(this, &ACPlayer::OnMovementTypeChanged);
	Action->OnActionTypeChanged.AddDynamic(this, &ACPlayer::OnActionTypeChanged);
	//State->OnStateTypeChanged.Clear();

	StatusBar = CreateWidget<UCUserWidget_StatusBar, APlayerController>(GetController<APlayerController>(), StatusBarWidgetClass);
	StatusBar->AddToViewport();
	StatusBar->UpdateHealthBar(Status->GetHealth(), Status->GetMaxHealth());
	StatusBar->UpdateStaminaBar(Status->GetStamina(), Status->GetMaxStamina());
	StatusBar->UpdateManaBar(Status->GetMana(), Status->GetMaxMana());

	Action->SetUnarmedMode();
	
	OffRun();

	bCooldown = false;

	bCooldown_Skill = false;
	Cooldown_SkillAction = 0.0f;

	bCooldown_Special = false;
	Cooldown_SpecialAction = 0.0f;
}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckNull(Status);
	CheckNull(StatusBar);
	CheckNull(Action->GetCurrent()->GetDoAction());

	if (Status->GetHealth() < Status->GetMaxHealth())
	{
		Status->AddHealth(5.0f * DeltaTime);
	}

	if (Status->GetMana() < Status->GetMaxMana())
	{
		Status->AddMana(30.0f * DeltaTime);
	}

	if (State->IsRunning() == true)
	{
		Status->SubStamina(50.0f * DeltaTime);
	}
	else
	{
		Status->AddStamina(50.0f * DeltaTime);
	}

	if (State->IsSprinting() == true)
	{
		Status->SubStamina(200.0f * DeltaTime);
	}

	StatusBar->UpdateHealthBar(Status->GetHealth(), Status->GetMaxHealth());
	StatusBar->UpdateManaBar(Status->GetMana(), Status->GetMaxMana());
	StatusBar->UpdateStaminaBar(Status->GetStamina(), Status->GetMaxStamina());

	if (Status->GetStamina() < 10.0f)
	{
		OffRun();
	}

	if (bCooldown_Skill == true)
	{
		Cooldown_SkillAction += DeltaTime;
		//CLog::Print(Cooldown_SkillAction);

		StatusBar->UpdateCooldown_Dual_Skill(MaxCooldown_SkillAction, Cooldown_SkillAction, false);

		if (Action->GetCurrent()->GetDoAction()->GetReady() == false)
		{
			StatusBar->UpdateCooldown_Archer_Skill(MaxCooldown_SkillAction, Cooldown_SkillAction, false);
		}

		//StatusBar
		if (Cooldown_SkillAction >= MaxCooldown_SkillAction)
		{
			bCooldown_Skill = false;

			StatusBar->UpdateCooldown_Dual_Skill(MaxCooldown_SkillAction, Cooldown_SkillAction, true);
			StatusBar->UpdateCooldown_Archer_Skill(MaxCooldown_SkillAction, Cooldown_SkillAction, true);

			Cooldown_SkillAction = 0.0f;
		}
	}

	if (bCooldown_Special == true)
	{
		Cooldown_SpecialAction += DeltaTime;

		StatusBar->UpdateCooldown_Dual_Special(MaxCooldown_SpecialAction, Cooldown_SpecialAction, false);
		StatusBar->UpdateCooldown_Archer_Special(MaxCooldown_SpecialAction, Cooldown_SpecialAction, false);

		if (Cooldown_SpecialAction >= MaxCooldown_SpecialAction)
		{
			bCooldown_Special = false;

			StatusBar->UpdateCooldown_Dual_Special(MaxCooldown_SkillAction, Cooldown_SkillAction, true);
			StatusBar->UpdateCooldown_Archer_Special(MaxCooldown_SpecialAction, Cooldown_SpecialAction, true);

			Cooldown_SpecialAction = 0.0f;
		}
	}
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement Action
	PlayerInputComponent->BindAxis("HorizontalLook", this, &ACPlayer::OnHorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", this, &ACPlayer::OnVerticalLook);
	PlayerInputComponent->BindAxis("MoveForward", this, &ACPlayer::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPlayer::OnMoveRight);

	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Pressed, this, &ACPlayer::OnRun);
	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Released, this, &ACPlayer::OffRun);

	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACPlayer::OnJump);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &ACPlayer::OffJump);

	PlayerInputComponent->BindAction("TurnBack", EInputEvent::IE_Released, this, &ACPlayer::OnTurnBack);
	PlayerInputComponent->BindAction("TurnForward", EInputEvent::IE_Released, this, &ACPlayer::OnTurnForward);
	PlayerInputComponent->BindAction("TurnJump", EInputEvent::IE_Released, this, &ACPlayer::OnTurnJump);

	PlayerInputComponent->BindAction("TurnParkour", EInputEvent::IE_Pressed, this, &ACPlayer::OnTurnParkour);

	// Change Action Type
	PlayerInputComponent->BindAction("Dual", EInputEvent::IE_Pressed, this, &ACPlayer::OnDual);
	PlayerInputComponent->BindAction("Archer", EInputEvent::IE_Pressed, this, &ACPlayer::OnArcher);

	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, this, &ACPlayer::OnDoAction);
	PlayerInputComponent->BindAction("AirAction", EInputEvent::IE_Pressed, this, &ACPlayer::OnAirAction);
	PlayerInputComponent->BindAction("SkillAction", EInputEvent::IE_Pressed, this, &ACPlayer::OnSkillAction);

	PlayerInputComponent->BindAction("GrappleHook", EInputEvent::IE_Pressed, this, &ACPlayer::OnGrapple);

	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Pressed, this, &::ACPlayer::OnAim);
	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Released, this, &::ACPlayer::OffAim);

	PlayerInputComponent->BindAction("Crouch", EInputEvent::IE_Pressed, this, &::ACPlayer::OnCrouch);
	PlayerInputComponent->BindAction("Crouch", EInputEvent::IE_Released, this, &::ACPlayer::OffCrouch);

	PlayerInputComponent->BindAction("Takedown", EInputEvent::IE_Pressed, this, &::ACPlayer::OnTakedownAction);

	PlayerInputComponent->BindAction("Target", EInputEvent::IE_Pressed, this, &::ACPlayer::OnTarget);
	
	PlayerInputComponent->BindAction("Special", EInputEvent::IE_Pressed, this, &::ACPlayer::OnSpecialAction);

	PlayerInputComponent->BindAction("Guard", EInputEvent::IE_Pressed, this, &::ACPlayer::OnGuard);
	PlayerInputComponent->BindAction("Parry", EInputEvent::IE_Pressed, this, &::ACPlayer::OnParry);

	PlayerInputComponent->BindAction("Dash", EInputEvent::IE_Pressed, this, &::ACPlayer::OnDash);

	PlayerInputComponent->BindAction("Riding", EInputEvent::IE_Pressed, this, &::ACPlayer::OnRiding);

	PlayerInputComponent->BindAction("ZoomIn", EInputEvent::IE_Pressed, this, &ACPlayer::OnZoomIn);
	PlayerInputComponent->BindAction("ZoomOut", EInputEvent::IE_Pressed, this, &ACPlayer::OnZoomOut);

	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &ACPlayer::OnSprint);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &ACPlayer::OffSprint);

}

FGenericTeamId ACPlayer::GetGenericTeamId() const
{
	return FGenericTeamId(TeamID);
}

void ACPlayer::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
	if (State->IsFlying() == true && PrevMovementMode == EMovementMode::MOVE_Falling)
	{
		//CLog::Log("OnMovementModeChanged Land");
		Montage->PlayLand();
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	}
}

float ACPlayer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* PawnInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(Damage, DamageEvent, PawnInstigator, DamageCauser);

	if(State->IsParry())
	{
		return Status->GetHealth();
	}
	
	DamageInstigator = PawnInstigator;

	Status->SubHealth(damage);
	if (Status->GetHealth() <= 0.0f)
	{
		State->SetDeadMode();

		return Status->GetHealth();
	}

	if (State->IsSkillAction() == true || State->IsSpecialAction() == true)
	{
		//Cast<UCUserWidget_HealthBar>(HealthWidget->GetUserWidgetObject())->Update(Status->GetHealth(), Status->GetMaxHealth());
		StatusBar->UpdateHealthBar(Status->GetHealth(), Status->GetMaxHealth());

		return Status->GetHealth();
	}

	if (State->IsAction() == true || State->IsAirAction() == true || State->IsAimAndOverdraw() == true)
	{
		Action->GetCurrent()->GetDoAction()->Reset();
	}

	State->SetHittedMode();

	return Status->GetHealth();
}

void ACPlayer::OnHorizontalLook(float Axis)
{
	CheckNull(Option);
	CheckTrue(bSequence == true);
	
	float rate = Option->GetHorizontalLookRate();
	AddControllerYawInput(Axis * rate * GetWorld()->GetDeltaSeconds());
}

void ACPlayer::OnVerticalLook(float Axis)
{
	CheckNull(Option);
	CheckTrue(bSequence == true);

	float rate = Option->GetVerticalLookRate();
	AddControllerPitchInput(Axis * rate * GetWorld()->GetDeltaSeconds());
}

void ACPlayer::OnMoveForward(float Axis)
{
	CheckNull(State);
	CheckNull(Status);
	CheckNull(Parkour);
	CheckTrue(State->IsDead());
	CheckFalse(Status->GetCanMove());
	CheckTrue(State->IsParkour());
	CheckTrue(Parkour->IsHanging());
	CheckTrue(State->IsRiding_Movement());

	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector();

	AddMovementInput(direction, Axis);
}

void ACPlayer::OnMoveRight(float Axis)
{
	CheckNull(State);
	CheckNull(Status);
	CheckNull(Parkour);
	CheckTrue(State->IsDead());
	CheckFalse(Status->GetCanMove());
	CheckTrue(State->IsParkour());
	CheckTrue(Parkour->IsHanging());
	CheckTrue(State->IsRiding_Movement());

	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector();

	AddMovementInput(direction, Axis);
}

void ACPlayer::OnZoomIn()
{
	CheckNull(Camera);
	CheckNull(SpringArm);
	float targetArmLength = SpringArm->TargetArmLength;
	targetArmLength -= 50.0f;
	SpringArm->TargetArmLength = UKismetMathLibrary::Clamp(targetArmLength, 500, 1000);
	float cameraHeight = Camera->GetRelativeLocation().Z;
	cameraHeight -= 10.0f;
	Camera->SetRelativeLocation(FVector(0.0f, 0.0f, UKismetMathLibrary::Clamp(cameraHeight, 140, 240)));
}

void ACPlayer::OnZoomOut()
{
	CheckNull(Camera);
	CheckNull(SpringArm);
	float targetArmLength = SpringArm->TargetArmLength;
	targetArmLength += 50.0f;
	SpringArm->TargetArmLength = UKismetMathLibrary::Clamp(targetArmLength, 500, 1000);
	float cameraHeight = Camera->GetRelativeLocation().Z;
	cameraHeight += 10.0f;
	Camera->SetRelativeLocation(FVector(0.0f, 0.0f, UKismetMathLibrary::Clamp(cameraHeight, 140, 240)));
}

void ACPlayer::OnRun()
{
	CheckNull(State);
	CheckTrue(State->IsDead());
	CheckTrue(State->IsRiding_Movement());
	CheckFalse(State->IsIdle());

	if(Status->GetStamina() < 10.0f)
	{
		//OffRun();
		return;
	}
	GetCharacterMovement()->MaxWalkSpeed = Status->GetRunSpeed();
	State->SetRunningMode();
}

void ACPlayer::OffRun()
{
	CheckNull(State);
	GetCharacterMovement()->MaxWalkSpeed = Status->GetWalkSpeed();
	State->SetWalkingMode();
}

void ACPlayer::OnSprint()
{
	CheckNull(State);
	CheckNull(Action);
	CheckTrue(State->IsDead());
	CheckTrue(State->IsRiding_Movement());
	CheckFalse(Action->IsArcherMode());
	CheckFalse(State->IsIdle());


	if (Status->GetStamina() < 10.0f)
	{
		//OffRun();
		return;
	}

	GetCharacterMovement()->MaxWalkSpeed = Status->GetSprintSpeed();
	State->SetSprintingMode();
}

void ACPlayer::OffSprint()
{
	CheckNull(State);
	GetCharacterMovement()->MaxWalkSpeed = Status->GetWalkSpeed();
	State->SetWalkingMode();
}

void ACPlayer::OnJump()
{
	CheckNull(State);
	CheckNull(Parkour);
	CheckNull(Action);
	CheckNull(Montage);
	CheckTrue(State->IsDead());
	CheckFalse(Status->GetCanMove());
	CheckTrue(State->IsRiding_Movement());
	CheckTrue(Action->IsArcherMode());

	//CheckFalse(Action->IsUnarmedMode() == true);

	float rightAxis = Parkour->RightAxis();

	if (Parkour->IsHanging() == true)
	{
		CheckTrue(Action->IsDualMode() == true);

		if (Parkour->GetCanJumpRight() == true)
		{
			if (rightAxis > 0.0f)
			{
				GetController()->DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
				//OwnerCharacter->GetController()->EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
				Parkour->JumpRightGrabLedge();
			}
			else
			{
				Parkour->ClimbLedge();
			}
		}
		else
		{
			if (Parkour->GetCanJumpLeft() == true)
			{
				if (rightAxis < 0.0f)
				{
					GetController()->DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
					//OwnerCharacter->GetController()->EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
					Parkour->JumpLeftGrabLedge();
				}
				else
				{
					//CLog::Print("climb true");
					Parkour->ClimbLedge();
				}
			}
			else
			{
				if (Parkour->GetCanJumpUp() == true)
				{
					Parkour->JumpUpLedge();
				}
				else
				{
					Parkour->ClimbLedge();
				}
			}
		}
	} // 	if (Parkour->IsHanging() == true)
	else
	{
		if (Parkour->CanGoUpWall() == true && Parkour->CanJumpOver() == false)
		{
			CheckTrue(Action->IsDualMode() == true);
			CheckFalse(Action->IsUnarmedMode() == true);
		
			//CLog::Log("GoUpWall");
			Parkour->GoUpWall();
		}
		else if (Parkour->CanJumpOver() == true)
		{
			CheckTrue(Action->IsDualMode() == true);
			CheckFalse(Action->IsUnarmedMode() == true);
		
			//CLog::Log("JumpOver");
			Parkour->JumpOver();
		}
		else
		{
			//Feet->SetStop(true);
			if (Parkour->CanWallRun() == true)
			{
				CheckTrue(Action->IsDualMode() == true);
				CheckFalse(Action->IsUnarmedMode() == true);

				Parkour->WallRun();
			}
			else
			{
				if (Action->IsUnarmedMode() == true || Action->IsDualMode() == true)
				{
					Jump();
					UKismetSystemLibrary::K2_SetTimer(this, "SetJumpCount", 0.2f, false);
				}

				if (JumpCount != 0 && Action->IsDualMode() == true)
				{
					if (JumpCount < 2)
					{
						Montage->PlayDoubleJump();
						
						FVector direction = GetActorForwardVector();
						LaunchCharacter(direction * 300.0f, true, false);
						direction = GetActorUpVector();
						LaunchCharacter(direction * 500.0f, false, true);

						JumpCount++;
						bResetJumpCount = true;
						UKismetSystemLibrary::K2_SetTimer(this, "SetJumpCount", 0.1f, false);
					}
				}
			}
		} // if (Parkour->CanGoUpWall() == true && Parkour->CanJumpOver() == false)

	} // else
}

void ACPlayer::SetJumpCount()
{
	JumpCount++;
	//CLog::Log(JumpCount);

	if (bResetJumpCount == true)
	{
		JumpCount = 0;
		bResetJumpCount = false;
		//CLog::Log(JumpCount);
	}
}

void ACPlayer::OffJump()
{
	CheckNull(State);
	CheckFalse(Status->GetCanMove());

	StopJumping();
}

void ACPlayer::OnTurnBack()
{
	CheckNull(State);
	CheckTrue(State->IsDead());

	Parkour->TurnedBack();
}

void ACPlayer::OnTurnParkour()
{
	CheckNull(State);
	CheckTrue(State->IsDead());

	Parkour->TurnParkour();
}

void ACPlayer::OnTurnForward()
{
	CheckNull(State);
	CheckTrue(State->IsDead());

	Parkour->TurnedForward();
}

void ACPlayer::OnTurnJump()
{
	CheckNull(State);
	CheckTrue(State->IsDead());

	Parkour->TurnedJump();
}

void ACPlayer::OnRiding()
{
	CheckNull(State);
	CheckNull(Riding);
	CheckTrue(State->IsDead());
	CheckFalse(Action->IsUnarmedMode());

	if (State->IsRiding_Movement() == true)
	{
		Riding->OffRiding();
	}
	else
	{
		Riding->OnRiding();
	}
}

void ACPlayer::OnDash()
{
	CheckNull(State);
	CheckNull(Action);
	CheckNull(Montage);
	CheckTrue(State->IsDead());
	CheckTrue(State->IsGuarding());
	CheckFalse(State->IsIdle());
	CheckFalse(Action->IsDualMode());
	
	State->SetStepMode();

	Montage->PlayDash();
	
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FLatentActionInfo info;
	info.CallbackTarget = this;
	info.UUID = GetUniqueID();
	info.ExecutionFunction = "EndDash";
	info.Linkage = 0;

	FVector dashLocation = GetActorLocation();
	FVector direction = GetActorForwardVector();
	dashLocation += direction * 400.0f;

	FRotator dashRotation = GetActorRotation();

	UKismetSystemLibrary::MoveComponentTo
	(
		GetCapsuleComponent(),
		dashLocation,
		dashRotation,
		false,
		false,
		0.3f,
		false,
		EMoveComponentAction::Move,
		info
	);
}

void ACPlayer::EndDash()
{
	CheckNull(State);
	// OnDash() - MoveComponentTo_ExecutionFunction 
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	State->SetIdleMode();
}

void ACPlayer::OnDual()
{
	CheckNull(Action);
	CheckNull(State);
	CheckTrue(State->IsDead());
	CheckFalse(State->IsIdle());
	CheckTrue(State->IsRiding_Movement());

	Action->SetDualMode();
}

void ACPlayer::OnArcher()
{
	CheckNull(Action);
	CheckNull(State);
	CheckTrue(State->IsDead());
	CheckFalse(State->IsIdle());
	CheckTrue(State->IsRiding_Movement());

	if (State->IsFlying() == true)
	{
		bAirSwap = true;
	}

	Action->SetArcherMode();
}

void ACPlayer::OnDoAction()
{
	CheckNull(Action);
	CheckNull(State);
	CheckTrue(State->IsDead());
	CheckTrue(State->IsGuarding());
	CheckTrue(Action->IsUnarmedMode());

	Action->DoAction();
}

void ACPlayer::OnAirAction()
{
	CheckNull(Action);
	CheckNull(State);
	CheckTrue(State->IsDead());
	CheckTrue(Action->IsUnarmedMode());

	Action->AirAction();
}

void ACPlayer::OnSkillAction()
{
	CheckNull(Action);
	CheckNull(State);
	CheckTrue(State->IsDead());
	CheckTrue(Action->IsUnarmedMode());

	if (bCooldown_Skill == true)
	{
		if (Action->IsDualMode() == true)
		{
			StatusBar->OnCantUse_DualSkill();
		}

		if (Action->IsArcherMode() == true)
		{
			StatusBar->OnCantUse_ArcherSkill();
		}

		if (Shake != NULL)
		{
			TSubclassOf<UMatineeCameraShake> shake = Shake;
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->StartCameraShake(shake);
		}

		//StatusBar->OnCantUse(true, false, Action->IsDualMode(), Action->IsArcherMode());
		return;
	}

	if (Status->GetMana() < ManaConsumeValue)
	{
		if (Shake != NULL)
		{
			TSubclassOf<UMatineeCameraShake> shake = Shake;
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->StartCameraShake(shake);
		}
		
		StatusBar->OnLackMana();
		
		return;
	}

	CheckFalse(State->IsIdle());

	Action->SkillAction();

	Status->SubMana(ManaConsumeValue);
	StatusBar->UpdateManaBar(Status->GetMana(), Status->GetMaxMana());
	
	bCooldown_Skill = true;
}

void ACPlayer::OnTakedownAction()
{
	CheckNull(Action);
	CheckNull(State);
	CheckTrue(State->IsDead());

	if (Action->IsUnarmedMode() == true || Action->IsArcherMode() == true)
	{
		Action->TakedownAction();
	}
}

void ACPlayer::OnSpecialAction()
{
	CheckNull(Action);
	CheckNull(State);
	CheckTrue(State->IsDead());
	CheckTrue(Action->IsUnarmedMode());

	if (bCooldown_Special == true)
	{
		if (Action->IsDualMode() == true)
		{
			StatusBar->OnCantUse_DualSpecial();
		}

		if (Action->IsArcherMode() == true)
		{
			StatusBar->OnCantUse_ArcherSpecial();
		}

		if (Shake != NULL)
		{
			TSubclassOf<UMatineeCameraShake> shake = Shake;
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->StartCameraShake(shake);
		}

		//StatusBar->OnCantUse(true, false, Action->IsDualMode(), Action->IsArcherMode());
		return;
	}

	if (Status->GetMana() < ManaConsumeValue)
	{
		if (Shake != NULL)
		{
			TSubclassOf<UMatineeCameraShake> shake = Shake;
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->StartCameraShake(shake);
		}
		
		StatusBar->OnLackMana();
		
		return;
	}

	CheckFalse(State->IsIdle());

	Action->SpecialAction();
	Status->SubMana(ManaConsumeValue);
	StatusBar->UpdateManaBar(Status->GetMana(), Status->GetMaxMana());

	EActionType actionType = Action->GetActionType();
	switch (actionType)
	{
		case EActionType::Dual:
		{
			OnSequence_Dual();
			bSequence = true;

		}
			break;
	}

	bCooldown_Special = true;
}

void ACPlayer::OnAim()
{
	CheckNull(Action);
	CheckNull(State);
	CheckTrue(State->IsDead());
	CheckFalse(Action->IsArcherMode());

	Action->OnAim();
}

void ACPlayer::OffAim()
{
	CheckNull(Action);
	CheckNull(State);
	CheckTrue(State->IsDead());
	CheckFalse(Action->IsArcherMode());

	Action->OffAim();
}

void ACPlayer::Hitted()
{
	CheckNull(Action);
	CheckNull(State);
	CheckTrue(State->IsDead());

	if(State->IsGuarding())
	{
		Action->Guard();

		return;
	}

	StatusBar->UpdateHealthBar(Status->GetHealth(), Status->GetMaxHealth());
	
	Montage->PlayHitted();
}

void ACPlayer::Dead()
{
	CheckNull(Action);
	CheckNull(State);
	Action->Dead();
	Montage->PlayDead();

	if (NiagaraEffect != NULL)
	{
		UNiagaraComponent* effect = UNiagaraFunctionLibrary::SpawnSystemAttached
		(
			NiagaraEffect,
			GetMesh(),
			"",
			GetActorLocation(),
			GetActorRotation(),
			EAttachLocation::KeepRelativeOffset,
			true
		);
	}
	Action->End_Dead();

}

void ACPlayer::OnGrapple()
{
	CheckNull(Grapple);
	CheckNull(State);
	CheckTrue(State->IsDead());

	CheckFalse(State->IsIdle());
	Grapple->OnGrapple();
}

void ACPlayer::Walking()
{
	CheckNull(State);
	CheckTrue(State->IsDead());

}

void ACPlayer::Flying()
{
	CheckNull(State);
	CheckTrue(State->IsDead());

}

void ACPlayer::Crouching()
{
	CheckNull(State);
	CheckTrue(State->IsDead());

}

void ACPlayer::OnTarget()
{
	CheckNull(State);
	CheckNull(Target);
	CheckTrue(State->IsDead());


	Target->OnTarget();
}

void ACPlayer::OnGuard()
{
	CheckNull(State);
	CheckNull(Action);
	CheckTrue(State->IsDead());
	CheckFalse(Action->IsDualMode());
	
	if(State->IsGuarding() == true)
	{
		State->SetRunningMode();
		State->SetIdleMode();
		return;
	}
	
	CheckFalse(State->IsIdle());
	Action->Guard();
	//CLog::Print("Player GuardAction");
}

void ACPlayer::OnParry()
{
	CheckNull(State);
	CheckNull(Action);
	CheckTrue(State->IsDead());
	CheckFalse(State->IsGuarding());
	CheckFalse(Action->IsDualMode() == true);

	Action->Parry();
}

void ACPlayer::OnCrouch()
{
	CheckNull(State);
	CheckNull(Action);
	CheckTrue(State->IsDead());
	CheckTrue(Action->IsDualMode());
	CheckFalse(State->IsIdle());

	State->SeCrouchingMode();

	if (Action->IsUnarmedMode())
	{
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	Crouch();
}

void ACPlayer::OffCrouch()
{
	CheckNull(State);
	CheckNull(Action);
	CheckFalse(State->IsCrouching());
	State->SetWalkingMode();

	if (Action->IsUnarmedMode())
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}

	UnCrouch();
}

void ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
		case EStateType::Idle:
		{
			if (InPrevType == EStateType::Parkour)
			{
				SpringArm->bDoCollisionTest = true;
			}

			if (InPrevType == EStateType::SpecialAction && Action->IsDualMode() == true)
			{
				OffSequence_Dual();
			}

			bSequence = false;

			
		}
			break;
	
		case EStateType::Hitted:
		{
			Hitted();
		}
			break;

		case EStateType::Parkour:
		{
			SpringArm->bDoCollisionTest = false;
		}
			break;

	}
}

void ACPlayer::OnMovementTypeChanged(EMovementType InPrevType, EMovementType InNewType)
{
	switch (InNewType)
	{
	case EMovementType::Walking:
		Walking();
		break;

	case EMovementType::Flying:
		Flying();
		break;

	case EMovementType::Crouching:
		Crouching();
		break;
	}
}

void ACPlayer::OnActionTypeChanged(EActionType InPrevType, EActionType InNewType)
{
	switch (InNewType)
	{
		case EActionType::Unarmed:
			StatusBar->OnUnarmedMode();
			SpringArm->bDoCollisionTest = true;
			break;
		case EActionType::Dual:
			StatusBar->OnDualMode();
			SpringArm->bDoCollisionTest = false;
			break;
		case EActionType::Archer:
			StatusBar->OnArcherMode();
			SpringArm->bDoCollisionTest = false;
			break;
		default:
			break;
	}
}

void ACPlayer::StartIntro()
{
	StatusBar->SetVisibility(ESlateVisibility::Hidden);
}

void ACPlayer::EndIntro()
{
	StatusBar->SetVisibility(ESlateVisibility::Visible);
	StatusBar->AppearHealthBar_Boss();
}

