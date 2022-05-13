#include "Characters/CEnemy_AI_Summon_GirlKngiht.h"
#include "Global.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CStatusComponent.h"
#include "Components/CActionComponent.h"
#include "Components/WidgetComponent.h"

#include "Actions/CDoAction.h"

#include "Widgets/CUserWidget_HealthBar.h"

ACEnemy_AI_Summon_GirlKngiht::ACEnemy_AI_Summon_GirlKngiht()
{
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/GirlKnight/Meshes/GK_Mesh_Red_SK.GK_Mesh_Red_SK'");
	GetMesh()->SetSkeletalMesh(mesh);
	GetMesh()->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));

	TSubclassOf<UAnimInstance> animInstance;
	CHelpers::GetClass<UAnimInstance>(&animInstance, "AnimBlueprint'/Game/Enemy/GirlKnight/ABP_GirlKnight.ABP_GirlKnight_C'");
	GetMesh()->SetAnimInstanceClass(animInstance);

	GetCharacterMovement()->MaxWalkSpeed = Status->GetRunSpeed();
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

	CHelpers::CreateComponent<UWidgetComponent>(this, &HealthWidget, "HealthWidget", GetMesh());

	TSubclassOf<UCUserWidget_HealthBar> healthClass;
	CHelpers::GetClass<UCUserWidget_HealthBar>(&healthClass, "WidgetBlueprint'/Game/Widgets/WB_HealthBar.WB_HealthBar_C'");
	HealthWidget->SetWidgetClass(healthClass);
	HealthWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 195.0f));
	HealthWidget->SetDrawSize(FVector2D(100.0f, 10.0f));
	HealthWidget->SetWidgetSpace(EWidgetSpace::Screen);

}

void ACEnemy_AI_Summon_GirlKngiht::BeginPlay()
{
	Super::BeginPlay();

	EnemyType = EEnemyType::GirlKnight;
	Action->SetTwoHandMode();

	HealthWidget->InitWidget();
	HealthWidget->SetVisibility(false);
	Cast<UCUserWidget_HealthBar>(HealthWidget->GetUserWidgetObject())->Update(Status->GetHealth(), Status->GetMaxHealth());

}

void ACEnemy_AI_Summon_GirlKngiht::OnAppear()
{
	HealthWidget->SetVisibility(true);
}

float ACEnemy_AI_Summon_GirlKngiht::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* PawnInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(Damage, DamageEvent, PawnInstigator, DamageCauser);

	DamageInstigator = PawnInstigator;

	Status->SubHealth(damage);

	if (Status->GetHealth() <= 0.0f)
	{
		Cast<UCUserWidget_HealthBar>(HealthWidget->GetUserWidgetObject())->Update(Status->GetHealth(), Status->GetMaxHealth());
		State->SetDeadMode();

		return Status->GetHealth();
	}

	if (State->IsAction() == true)
	{
		Action->GetCurrent()->GetDoAction()->Reset();
	}

	State->SetHittedMode();

	return Status->GetHealth();
}

void ACEnemy_AI_Summon_GirlKngiht::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
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

void ACEnemy_AI_Summon_GirlKngiht::OnMovementTypeChanged(EMovementType InPrevType, EMovementType InNewType)
{
	Super::OnMovementTypeChanged(InPrevType, InNewType);

	switch (InNewType)
	{
		case EMovementType::Walking:
			Walking();
			break;
		case EMovementType::Flying:
			Flying();
			break;
		case EMovementType::Crouching:
			break;
	}
}

void ACEnemy_AI_Summon_GirlKngiht::Hitted()
{
	Super::Hitted();
	CheckTrue(State->IsDead());

	Cast<UCUserWidget_HealthBar>(HealthWidget->GetUserWidgetObject())->Update(Status->GetHealth(), Status->GetMaxHealth());
	//CLog::Log("Dummy Call Hitted");

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

void ACEnemy_AI_Summon_GirlKngiht::AirHitted()
{
	Super::AirHitted();
}

void ACEnemy_AI_Summon_GirlKngiht::Dead()
{
	Super::Dead();

	HealthWidget->SetVisibility(false);
}

void ACEnemy_AI_Summon_GirlKngiht::Walking()
{
	Super::Walking();
}

void ACEnemy_AI_Summon_GirlKngiht::Flying()
{
	Super::Flying();
}


