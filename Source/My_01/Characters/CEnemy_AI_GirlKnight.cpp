#include "Characters/CEnemy_AI_GirlKnight.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CStatusComponent.h"
#include "Components/CActionComponent.h"
#include "Components/WidgetComponent.h"

#include "Actions/CDoAction.h"

#include "Widgets/CUserWidget_HealthBar.h"
#include "Widgets/CUserWidget_StatusBar.h"

#include "Characters/CPlayer.h"
#include "Characters/CAIController.h"


ACEnemy_AI_GirlKnight::ACEnemy_AI_GirlKnight()
{
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/GirlKnight/Meshes/GK_Mesh_White_SK.GK_Mesh_White_SK'");
	GetMesh()->SetSkeletalMesh(mesh);
	GetMesh()->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));

	TSubclassOf<UAnimInstance> animInstance;
	CHelpers::GetClass<UAnimInstance>(&animInstance, "AnimBlueprint'/Game/Enemy/GirlKnight/ABP_GirlKnight.ABP_GirlKnight_C'");
	GetMesh()->SetAnimInstanceClass(animInstance);
	
	GetCharacterMovement()->MaxWalkSpeed = Status->GetRunSpeed();
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);
}

void ACEnemy_AI_GirlKnight::BeginPlay()
{
	Super::BeginPlay();
	CheckNull(Action);

	EnemyType = EEnemyType::GirlKnight;
	Action->SetGreatSwordMode();
}

float ACEnemy_AI_GirlKnight::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* PawnInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(Damage, DamageEvent, PawnInstigator, DamageCauser);

	DamageInstigator = PawnInstigator;

	Status->SubHealth(damage);

	CLog::Log(Status->GetHealth());
	//CLog::Print(Status->GetHealth());

	AccumulatedDamage += damage;

	if (Status->GetHealth() <= 500.0f)
	{
		CLog::Log("GirlKnight State SetDead");

		// 스킬 혹은 스폐셜 액션 중 일때 죽이면 에셋 삭제 X
		State->SetDeadMode();

		return Status->GetHealth();
	}

	if (State->IsSkillAction() == true || State->IsSpecialAction() == true || State->IsSummonAction() == true)
	{
		if (StatusBar != NULL)
		{
			StatusBar->UpdateHealthBar_Boss(Status->GetHealth(), Status->GetMaxHealth());
		}

		return Status->GetHealth();
	}

	if (State->IsAction() == true || State->IsAirAction() == true)
	{
		Action->GetCurrent()->GetDoAction()->Reset();
	}

	State->SetHittedMode();

	return Status->GetHealth();
}

void ACEnemy_AI_GirlKnight::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	Super::OnStateTypeChanged(InPrevType, InNewType);

	switch (InNewType)
	{
		case EStateType::Hitted:
		{
			Hitted();
		}
			break;

		case EStateType::Dead:
		{
			Dead();
		}
			break;

	}
}

void ACEnemy_AI_GirlKnight::OnMovementTypeChanged(EMovementType InPrevType, EMovementType InNewType)
{
	Super::OnMovementTypeChanged(InPrevType, InNewType);

	switch (InNewType)
	{
	case EMovementType::Walking:
		break;
	case EMovementType::Flying:
		break;
	case EMovementType::Crouching:
		break;
	}
}

void ACEnemy_AI_GirlKnight::Hitted()
{
	Super::Hitted();
	CheckNull(State);
	CheckNull(StatusBar);
	CheckTrue(State->IsDead());

	StatusBar->UpdateHealthBar_Boss(Status->GetHealth(), Status->GetMaxHealth());
	
	ACharacter* attacker = Cast<ACharacter>(DamageInstigator->GetPawn());
	CheckNull(attacker);

	FVector start = GetActorLocation();
	start.Z = 0.0f;
	FVector target = attacker->GetActorLocation();
	target.Z = 0.0f;

	FRotator rotation = UKismetMathLibrary::FindLookAtRotation(start, target);
	rotation.Normalize();

	SetActorRotation(rotation);

	Montage->PlayHitted();
}

void ACEnemy_AI_GirlKnight::AirHitted()
{
	Super::AirHitted();
}

void ACEnemy_AI_GirlKnight::Dead()
{
	Super::Dead();
}

void ACEnemy_AI_GirlKnight::Walking()
{
	Super::Walking();
}

void ACEnemy_AI_GirlKnight::Flying()
{
	Super::Flying();
}
