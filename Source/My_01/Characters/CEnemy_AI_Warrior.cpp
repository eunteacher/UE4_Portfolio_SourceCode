#include "Characters/CEnemy_AI_Warrior.h"
#include "Global.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CStatusComponent.h"
#include "Components/CActionComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CMontageComponent.h"
#include "Components/CBehaviorCompoent.h"

#include "Actions/CDoAction.h"

#include "Widgets/CUserWidget_HealthBar.h"

#include "Particles/ParticleSystemComponent.h"

#include "Characters/CPlayer.h"

#include "Widgets/CUserWidget_StatusBar.h"

ACEnemy_AI_Warrior::ACEnemy_AI_Warrior()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	// SkeletalMesh'/Game/GKnight/Meshes/SK_GothicKnight_VA.SK_GothicKnight_VA'
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/GKnight/Meshes/SK_GothicKnight_VA.SK_GothicKnight_VA'");
	GetMesh()->SetSkeletalMesh(mesh);

	//AnimBlueprint'/Game/Enemy/Warrior/ABP_Warrior.ABP_Warrior'
	TSubclassOf<UAnimInstance> animInstance;
	CHelpers::GetClass<UAnimInstance>(&animInstance, "AnimBlueprint'/Game/Enemy/Warrior/ABP_Warrior.ABP_Warrior_C'");
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

void ACEnemy_AI_Warrior::BeginPlay()
{
	Super::BeginPlay();
	
	Action->SetWarriorMode();
	EnemyType = EEnemyType::Knight;

	HealthWidget->InitWidget();
	Cast<UCUserWidget_HealthBar>(HealthWidget->GetUserWidgetObject())->Update(Status->GetHealth(), Status->GetMaxHealth());

	HealthWidget->SetVisibility(false);
}

void ACEnemy_AI_Warrior::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

float ACEnemy_AI_Warrior::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* PawnInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(Damage, DamageEvent, PawnInstigator, DamageCauser);

	DamageInstigator = PawnInstigator;

	if (Montage->GetDeadType() == EDeadType::Takedown)
	{
		HealthWidget->SetVisibility(false);

		return Status->GetHealth();
	}

	if (State->IsGuarding() == true && State->IsDead() == false)
	{
		if (BlockCount >= 3)
		{
			State->SetWalkingMode();
			BlockCount = 0;
		}
		else
		{
			State->SetBlockMode();

			return Status->GetHealth();
		}
	}

	Status->SubHealth(damage);

	if (Status->GetHealth() <= 0.0f)
	{
		//CLog::Print("SetDeadMode()");
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

void ACEnemy_AI_Warrior::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
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

		case EStateType::Block:
		{
			Block();
		}
			break;
	}
}

void ACEnemy_AI_Warrior::OnMovementTypeChanged(EMovementType InPrevType, EMovementType InNewType)
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

		case EMovementType::Running:
			Running();
			break;
	
		case EMovementType::Guarding:
			Guarding();
			break;
	}
}

void ACEnemy_AI_Warrior::OnTakedownBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnTakedownBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	CheckTrue(State->IsDead() == true);

	//CLog::Log(OtherActor->GetName());

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

void ACEnemy_AI_Warrior::OnTakedownEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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

void ACEnemy_AI_Warrior::Hitted()
{
	Super::Hitted();
	CheckNull(State);
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

void ACEnemy_AI_Warrior::AirHitted()
{
	Super::AirHitted();
}

void ACEnemy_AI_Warrior::Dead()
{
	Super::Dead();


	HealthWidget->SetVisibility(false);
}

void ACEnemy_AI_Warrior::Walking()
{
	Super::Walking();
}

void ACEnemy_AI_Warrior::Running()
{
	Super::Running();
}

void ACEnemy_AI_Warrior::Flying()
{
	Super::Flying();
}

void ACEnemy_AI_Warrior::FindTarget()
{
	Super::FindTarget();

	HealthWidget->SetVisibility(true);
}

void ACEnemy_AI_Warrior::Guarding()
{
	CheckTrue(State->IsDead());

	Walking();
}

void ACEnemy_AI_Warrior::Block()
{
	CheckFalse(State->IsBlock() == true);
	CheckTrue(State->IsDead());

	BlockCount++;
	
	EHittedType hittedType = EHittedType::Block;
	EDeadType deadType = EDeadType::Dead;

	Montage->SetHittedAndDeadType(hittedType, deadType);
	Montage->PlayHitted();
}


