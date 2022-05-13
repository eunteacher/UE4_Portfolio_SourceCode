#include "Characters/CEnemy_AI_Archer.h"
#include "Global.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CStatusComponent.h"
#include "Components/CActionComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CBehaviorCompoent.h"

#include "Actions/CDoAction.h"
#include "Actions/CDoAction_Archer_Enemy.h"
#include "Actions/CFake.h"

#include "Widgets/CUserWidget_HealthBar.h"

#include "Characters/CPlayer.h"

#include "Widgets/CUserWidget_StatusBar.h"


ACEnemy_AI_Archer::ACEnemy_AI_Archer()
{
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	// SkeletalMesh'/Game/GKnight/Meshes/SK_GothicKnight_VA.SK_GothicKnight_VA'
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/GKnight/Meshes/SK_GothicKnight_VA.SK_GothicKnight_VA'");
	GetMesh()->SetSkeletalMesh(mesh);

	//AnimBlueprint'/Game/Enemy/Archer/ABP_Archer.ABP_Archer'
	TSubclassOf<UAnimInstance> animInstance;
	CHelpers::GetClass<UAnimInstance>(&animInstance, "AnimBlueprint'/Game/Enemy/Archer/ABP_Archer.ABP_Archer_C'");
	GetMesh()->SetAnimInstanceClass(animInstance);

	GetCharacterMovement()->MaxWalkSpeed = Status->GetRunSpeed();
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

	CHelpers::CreateComponent<UWidgetComponent>(this, &HealthWidget, "HealthWidget", GetMesh());

	TSubclassOf<UCUserWidget_HealthBar> healthClass;
	CHelpers::GetClass<UCUserWidget_HealthBar>(&healthClass, "WidgetBlueprint'/Game/Widgets/WB_HealthBar.WB_HealthBar_C'");
	HealthWidget->SetWidgetClass(healthClass);
	HealthWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 210.0f));
	HealthWidget->SetDrawSize(FVector2D(100.0f, 10.0f));
	HealthWidget->SetWidgetSpace(EWidgetSpace::Screen);
}

void ACEnemy_AI_Archer::BeginPlay()
{
	Super::BeginPlay();

	Action->SetArcherEnemyMode();
	EnemyType = EEnemyType::Knight;

	HealthWidget->InitWidget();
	Cast<UCUserWidget_HealthBar>(HealthWidget->GetUserWidgetObject())->Update(Status->GetHealth(), Status->GetMaxHealth());

	HealthWidget->SetVisibility(false);
}

void ACEnemy_AI_Archer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

float ACEnemy_AI_Archer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* PawnInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(Damage, DamageEvent, PawnInstigator, DamageCauser);

	DamageInstigator = PawnInstigator;

	if (Montage->GetDeadType() == EDeadType::Takedown)
	{
		HealthWidget->SetVisibility(false);

		return Status->GetHealth();
	}

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

void ACEnemy_AI_Archer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	Super::OnStateTypeChanged(InPrevType, InNewType);

	switch (InNewType)
	{
		case EStateType::Hitted:
		{
			Hitted();
		}
			break;

		case EStateType::Avoid:
		{
			Avoid();
		}
			break;

		case EStateType::Dead:
		{
			Dead();
		}
			break;
	}
}

void ACEnemy_AI_Archer::OnMovementTypeChanged(EMovementType InPrevType, EMovementType InNewType)
{
	Super::OnMovementTypeChanged(InPrevType, InNewType);

	switch (InNewType)
	{
		case EMovementType::Walking:
			Walking();
			break;

		case EMovementType::Running:
			Running();
			break;

		case EMovementType::Flying:
			break;

		case EMovementType::Crouching:
			break;
	}
}

void ACEnemy_AI_Archer::OnTakedownBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnTakedownBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	CheckTrue(State->IsDead());

	ACharacter* attacker = Cast<ACharacter>(OtherActor);
	UCActionComponent* actionAtt = CHelpers::GetComponent<UCActionComponent>(attacker);
	CheckNull(actionAtt);

	if (actionAtt->IsUnarmedMode() == true)
	{
		ACDoAction* attDoAction = Cast<ACDoAction>(actionAtt->GetCurrent()->GetDoAction());
		CheckNull(attDoAction);

		attDoAction->SetCanTakedonw(true);

		ACPlayer* player = Cast<ACPlayer>(attacker);
		CheckNull(player);

		player->GetStatusBar()->OnTakedown();
	}
}

void ACEnemy_AI_Archer::OnTakedownEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnTakedownEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	CheckTrue(State->IsDead());

	ACharacter* attacker = Cast<ACharacter>(OtherActor);
	UCActionComponent* actionAtt = CHelpers::GetComponent<UCActionComponent>(attacker);
	CheckNull(actionAtt);

	if (actionAtt->IsUnarmedMode() == true)
	{
		ACDoAction* attDoAction = Cast<ACDoAction>(actionAtt->GetCurrent()->GetDoAction());
		CheckNull(attDoAction);

		attDoAction->SetCanTakedonw(false);

		ACPlayer* player = Cast<ACPlayer>(attacker);
		CheckNull(player);

		player->GetStatusBar()->OffTakedown();
	}
}

void ACEnemy_AI_Archer::Hitted()
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

	if (Cast<ACDoAction_Archer_Enemy>(Action->GetCurrent()->GetDoAction()) != NULL)
	{
		ACDoAction_Archer_Enemy* archer = Cast<ACDoAction_Archer_Enemy>(Action->GetCurrent()->GetDoAction());

		if (archer->GetFakeArrow() != NULL)
		{
			if (archer->GetFakeArrow()->GetIsAppear() == true)
			{
				archer->GetFakeArrow()->SetDisappear();
				archer->GetFakeArrow()->OnDisappear();
				CLog::Log("Hitted Mode : FakeArrow Disappear");
			}
		}
	}

	Montage->PlayHitted();
}

void ACEnemy_AI_Archer::AirHitted()
{
	Super::AirHitted();
}

void ACEnemy_AI_Archer::Dead()
{
	Super::Dead();

	HealthWidget->SetVisibility(false);

	if (Cast<ACDoAction_Archer_Enemy>(Action->GetCurrent()->GetDoAction()) != NULL)
	{
		ACDoAction_Archer_Enemy* archer = Cast<ACDoAction_Archer_Enemy>(Action->GetCurrent()->GetDoAction());

		if (archer->GetFakeArrow() != NULL)
		{
			if (archer->GetFakeArrow()->GetIsAppear() == true)
			{
				archer->GetFakeArrow()->SetDisappear();
				archer->GetFakeArrow()->OnDisappear();
				CLog::Log("Avoid Mode : FakeArrow Disappear");
			}
		}
	}
}

void ACEnemy_AI_Archer::Walking()
{
	Super::Walking();
}

void ACEnemy_AI_Archer::Running()
{
	Super::Running();
}

void ACEnemy_AI_Archer::Flying()
{
	Super::Flying();
}

void ACEnemy_AI_Archer::Avoid()
{
	CheckTrue(State->IsDead());
	CheckTrue(State->IsIdle());

	if (Cast<ACDoAction_Archer_Enemy>(Action->GetCurrent()->GetDoAction()) != NULL)
	{
		ACDoAction_Archer_Enemy* archer = Cast<ACDoAction_Archer_Enemy>(Action->GetCurrent()->GetDoAction());

		if (archer->GetFakeArrow() != NULL)
		{
			if (archer->GetFakeArrow()->GetIsAppear() == true)
			{
				archer->GetFakeArrow()->SetDisappear();
				archer->GetFakeArrow()->OnDisappear();
				CLog::Log("Avoid Mode : FakeArrow Disappear");
			}
		}
	}

	UCBehaviorCompoent* behavior = CHelpers::GetComponent<UCBehaviorCompoent>(GetController());
	CheckNull(behavior);

	if (behavior->GetTargetPlayer() != NULL)
	{
		PlayerLocation = behavior->GetTargetPlayer()->GetActorLocation();
	}

	FVector start = GetActorLocation();
	FVector from = behavior->GetAvoidLocation();
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, from));

	Montage->PlayAvoid();
}

void ACEnemy_AI_Archer::FindTarget()
{
	Super::FindTarget();

	HealthWidget->SetVisibility(true);
}

