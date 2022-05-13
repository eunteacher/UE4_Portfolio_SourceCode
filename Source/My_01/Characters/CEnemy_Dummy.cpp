#include "Characters/CEnemy_Dummy.h"
#include "Global.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CStatusComponent.h"
#include "Components/CActionComponent.h"
#include "Components/WidgetComponent.h"

#include "Actions/CDoAction.h"

#include "Widgets/CUserWidget_HealthBar.h"

#include "Characters/CPlayer.h"
#include "Widgets/CUserWidget_StatusBar.h"

ACEnemy_Dummy::ACEnemy_Dummy()
{
	// Mesh 기본값 설정
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	//SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin_Female.SK_Mannequin_Female'
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin_Female.SK_Mannequin_Female'");
	GetMesh()->SetSkeletalMesh(mesh);

	TSubclassOf<UAnimInstance> animInstance;
	CHelpers::GetClass<UAnimInstance>(&animInstance, "AnimBlueprint'/Game/Enemy/Dummy/ABP_Enemy_Dummy.ABP_Enemy_Dummy_C'");
	GetMesh()->SetAnimInstanceClass(animInstance);

	GetCharacterMovement()->MaxWalkSpeed = Status->GetWalkSpeed();
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

	CHelpers::CreateComponent<UWidgetComponent>(this, &HealthWidget, "HealthWidget", GetMesh());
	
	TSubclassOf<UCUserWidget_HealthBar> healthClass;
	CHelpers::GetClass<UCUserWidget_HealthBar>(&healthClass, "WidgetBlueprint'/Game/Widgets/WB_HealthBar.WB_HealthBar_C'");
	HealthWidget->SetWidgetClass(healthClass);
	HealthWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 195.0f));
	HealthWidget->SetDrawSize(FVector2D(100.0f, 10.0f));
	HealthWidget->SetWidgetSpace(EWidgetSpace::Screen);
}

void ACEnemy_Dummy::BeginPlay()
{
	Super::BeginPlay();
	
	Action->SetUnarmedMode();
	EnemyType = EEnemyType::Dummy;

	HealthWidget->InitWidget();
	Cast<UCUserWidget_HealthBar>(HealthWidget->GetUserWidgetObject())->Update(Status->GetHealth(), Status->GetMaxHealth());
}

float ACEnemy_Dummy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* PawnInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(Damage, DamageEvent, PawnInstigator, DamageCauser);
	
	DamageInstigator = PawnInstigator;

	Status->SubHealth(damage);
	if (Status->GetHealth() <= 0.0f)
	{
		
		State->SetDeadMode();

		return 0.0f;
	}

	State->SetHittedMode();

	return Status->GetHealth();
}

void ACEnemy_Dummy::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	Super::OnStateTypeChanged(InPrevType, InNewType);

	switch (InNewType)
	{
	case EStateType::Hitted:
		Hitted();

		break;

	}
}

void ACEnemy_Dummy::OnMovementTypeChanged(EMovementType InPrevType, EMovementType InNewType)
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

void ACEnemy_Dummy::OnTakedownBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnTakedownBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	CLog::Log(OtherActor->GetName());
	
	ACharacter* attacker = Cast<ACharacter>(OtherActor);
	UCActionComponent* actionAtt = CHelpers::GetComponent<UCActionComponent>(attacker);
	CheckNull(actionAtt);

	if (actionAtt->IsUnarmedMode() == true || actionAtt->IsSpearMode() == true)
	{
		ACDoAction* attDoAction = Cast<ACDoAction>(actionAtt->GetCurrent()->GetDoAction());
		CheckNull(attDoAction);

		attDoAction->SetCanTakedonw(true);

		ACPlayer* player = Cast<ACPlayer>(attacker);
		CheckNull(player);

		player->GetStatusBar()->OnTakedown();
	}
	
}

void ACEnemy_Dummy::OnTakedownEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnTakedownEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	
	ACharacter* attacker = Cast<ACharacter>(OtherActor);
	UCActionComponent* actionAtt = CHelpers::GetComponent<UCActionComponent>(attacker);
	CheckNull(actionAtt);

	if(actionAtt->IsUnarmedMode() == true || actionAtt->IsSpearMode() == true)
	{
		ACDoAction* attDoAction = Cast<ACDoAction>(actionAtt->GetCurrent()->GetDoAction());
		CheckNull(attDoAction);

		attDoAction->SetCanTakedonw(false);

		ACPlayer* player = Cast<ACPlayer>(attacker);
		CheckNull(player);

		player->GetStatusBar()->OffTakedown();
	}
}

void ACEnemy_Dummy::Hitted()
{
	Super::Hitted();

	Cast<UCUserWidget_HealthBar>(HealthWidget->GetUserWidgetObject())->Update(Status->GetHealth(), Status->GetMaxHealth());
	//CLog::Log("Dummy Call Hitted");
	Montage->PlayHitted();
}

void ACEnemy_Dummy::AirHitted()
{
	Super::AirHitted();
}

void ACEnemy_Dummy::Dead()
{
	Super::Dead();
}

void ACEnemy_Dummy::Walking()
{
	Super::Walking();

}

void ACEnemy_Dummy::Flying()
{
	Super::Flying();

}