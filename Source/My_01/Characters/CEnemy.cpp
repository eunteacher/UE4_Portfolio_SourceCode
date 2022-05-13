#include "Characters/CEnemy.h"
#include "Global.h"

#include "Animation/AnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CMontageComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CFeetComponent.h"

#include "Components/DecalComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"

#include "Actions/CDoAction.h"

#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

ACEnemy::ACEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<UDecalComponent>(this, &Decal, "Decal", GetRootComponent());
	UMaterialInstanceConstant* m;
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&m, "MaterialInstanceConstant'/Game/Materials/M_Target_Inst.M_Target_Inst'");
	Decal->SetDecalMaterial(m);
	Decal->DecalSize = FVector(64.0f, 128.0f, 128.0f);
	Decal->SetRelativeLocation(FVector(0.0f, 0.0f, -125.0f));
	Decal->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
	Decal->SetVisibility(false);

	CHelpers::CreateComponent<UBoxComponent>(this, &TakedownBox, "TakedownBox", GetRootComponent());
	TakedownBox->SetRelativeLocation(FVector(-80.0f, 0.0f, 0.0f));
	TakedownBox->SetRelativeScale3D(FVector(1.75f, 1.75f, 2.75f));
	
	Montage = CreateDefaultSubobject<UCMontageComponent>("Montage");
	Status = CreateDefaultSubobject<UCStatusComponent>("Status");
	State = CreateDefaultSubobject<UCStateComponent>("State");
	Action = CreateDefaultSubobject<UCActionComponent>("Action");
}

void ACEnemy::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
	CheckNull(State);
	CheckNull(Montage);
	CheckTrue(State->IsDead());

	if (State->IsFlying() == true && PrevMovementMode == EMovementMode::MOVE_Falling)
	{
		Montage->PlayLand();
	}
}

void ACEnemy::BeginPlay()
{
	CheckNull(State);
	CheckNull(TakedownBox);

	State->OnStateTypeChanged.AddDynamic(this, &ACEnemy::OnStateTypeChanged);
	State->OnMovementTypeChanged.AddDynamic(this, &ACEnemy::OnMovementTypeChanged);

	TakedownBox->OnComponentBeginOverlap.AddDynamic(this, &ACEnemy::OnTakedownBeginOverlap);
	TakedownBox->OnComponentEndOverlap.AddDynamic(this, &ACEnemy::OnTakedownEndOverlap);

	Super::BeginPlay();
}

void ACEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckNull(State);
	CheckTrue(State->IsDead());

	float value = GetCharacterMovement()->Velocity.Z;

	if (value < -1400.0f)
	{
		if (WaveParticle != NULL)
		{
			FTransform transform;
			transform.AddToTranslation(GetActorLocation());
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WaveParticle, transform);
		}
	}
}

void ACEnemy::Walking()
{
	CheckNull(Status);

	Status->SetSpeed(ECharacterSpeed::Walk);
}

void ACEnemy::Running()
{
	CheckNull(Status);

	Status->SetSpeed(ECharacterSpeed::Run);
}

void ACEnemy::Dead()
{
	CheckNull(Montage);
	CheckNull(Action);
	
	CLog::Log(GetName());

	if (Montage->GetDeadType() == EDeadType::FlyDead)
	{
		FVector direction = GetActorForwardVector() * -1.0f + GetActorUpVector() * -1.0f;
		float power = 500.0f;
		LaunchCharacter(direction * power, true, true);
	}

	if (NiagaraEffect != NULL)
	{
		UNiagaraComponent* effect = UNiagaraFunctionLibrary::SpawnSystemAttached
		(
			NiagaraEffect,
			GetMesh(),
			"EffectSocket",
			GetActorLocation(),
			GetActorRotation(),
			EAttachLocation::KeepRelativeOffset,
			true
		);
	}

	Montage->PlayDead();
	Action->Dead();
}

void ACEnemy::End_Dead()
{
	CheckNull(Action);

	Action->End_Dead();
	Destroy();
}

void ACEnemy::Takedown_Dead()
{
	CheckNull(Action);

	if (NiagaraEffect != NULL)
	{
		UNiagaraComponent* effect = UNiagaraFunctionLibrary::SpawnSystemAttached
		(
			NiagaraEffect,
			GetMesh(),
			"EffectSocket",
			GetActorLocation(),
			GetActorRotation(),
			EAttachLocation::KeepRelativeOffset,
			true
		);
	}

	Action->Dead();
	Action->End_Dead();
}

