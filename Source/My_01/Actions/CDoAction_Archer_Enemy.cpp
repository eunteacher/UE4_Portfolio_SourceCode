#include "Actions/CDoAction_Archer_Enemy.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Components/SkeletalMeshComponent.h"

#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CMontageComponent.h"
#include "Components/CBehaviorCompoent.h"

#include "Actions/CThrow.h"
#include "Actions/CFake.h"

void ACDoAction_Archer_Enemy::BeginPlay()
{
	Super::BeginPlay();

	FTransform transform;
	FActorSpawnParameters params;
	params.Owner = OwnerCharacter;

	const FDoActionData& data = DoActionDatas[0];

	// 페이크 화살 생성
	FakeArrow = GetWorld()->SpawnActorDeferred<ACFake>(data.FakeClass, transform, OwnerCharacter, NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	FakeArrow->SetDisappear();
	FakeArrow->SetSpecialDisAppear();
	UGameplayStatics::FinishSpawningActor(FakeArrow, transform);
	
	FakeArrow->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), "ArrowSocket");
	FakeArrow->OnDisappear();
}

void ACDoAction_Archer_Enemy::DoAction()
{
	Super::DoAction();
	CheckNull(State);
	CheckNull(Status);
	CheckNull(FakeArrow);
	CheckNull(OwnerCharacter);
	CheckFalse(State->IsIdle());
	
	State->SetActionMode();

	const FDoActionData& data = DoActionDatas[0];
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);

	FakeArrow->SetAppear();
	FakeArrow->OnAppear();

	if (data.bCanMove == true)
		Status->SetMove();
	else
		Status->SetStop();
}

void ACDoAction_Archer_Enemy::Begin_DoAction()
{
	Super::Begin_DoAction();

	FakeArrow->SetDisappear();
	FakeArrow->OnDisappear();

	FTransform transform = OwnerCharacter->GetMesh()->GetSocketTransform("ArrowSocket");

	ACharacter* target = Cast<ACharacter>(Behavior->GetTargetPlayer());

	if (Behavior->GetBehaviorType() != EBehaviorType::Action || target == NULL)
	{
		End_DoAction();
		return;
	}

	FRotator r = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), target->GetActorLocation());
	transform.SetRotation(FQuat(r));

	FActorSpawnParameters params;
	params.Owner = OwnerCharacter;

	ThrowArrow = GetWorld()->SpawnActorDeferred<ACThrow>(DoActionDatas[0].ThrowClass, transform, OwnerCharacter, NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	ThrowArrow->OnThrowBeginOverlap.AddDynamic(this, &ACDoAction_Archer_Enemy::OnThrowBeginOverlap);
	
	ThrowArrow->GetProjectile()->InitialSpeed = 1200.0f;
	ThrowArrow->GetProjectile()->MaxSpeed = 1500.0f;
	ThrowArrow->SetIsArcher(true);

	UGameplayStatics::FinishSpawningActor(ThrowArrow, transform);
}

void ACDoAction_Archer_Enemy::End_DoAction()
{
	Super::End_DoAction();

	FakeArrow->SetDisappear();
	FakeArrow->OnDisappear();

	const FDoActionData& data = DoActionDatas[0];
	OwnerCharacter->StopAnimMontage(data.AnimMontage);
	
	State->SetIdleMode();
	Status->SetMove();

	LookAtPlayer();
}

void ACDoAction_Archer_Enemy::Reset()
{
	Super::Reset();

	const FDoActionData& data = DoActionDatas[0];
	OwnerCharacter->StopAnimMontage(data.AnimMontage);

	State->SetIdleMode();
	Status->SetMove();
}

void ACDoAction_Archer_Enemy::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOtherCharacter);
	CheckNull(InOtherCharacter);

	// Enemy끼리의 공격 방지 
	FString name = InOtherCharacter->GetName();
	CheckTrue(name.Contains("Enemy"));

	const FDoActionData& data = DoActionDatas[0];

	if (data.IsLaunch == true)
	{
		FVector deltaLocation = OwnerCharacter->GetActorForwardVector() * data.LaunchValue;
		InOtherCharacter->AddActorWorldOffset(deltaLocation);
	}

	if (data.bHitStop == true)
	{
		float hitStopTime = data.HitStopTime;

		if (data.bLast == true)
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), hitStopTime);
			UKismetSystemLibrary::K2_SetTimer(this, "HitStopRestore", hitStopTime, false);
		}
		else
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1e-3f);
			UKismetSystemLibrary::K2_SetTimer(this, "HitStopRestore", hitStopTime * 1e-3f, false);
		}
	} // if (InData.bHitStop == true)

	float power = data.Power;

	TArray<UParticleSystem*> hitEffects = data.HitEffects;
	for (UParticleSystem* hitEffect : hitEffects)
	{
		if (hitEffect != NULL)
		{
			FTransform transform = data.HitEffectTransform;
			transform.AddToTranslation(InOtherCharacter->GetActorLocation());
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, transform);
		}
	}

	EHittedType hittedType = data.HittedType;
	EDeadType deadType = data.DeadType;
	UCMontageComponent* montage = CHelpers::GetComponent<UCMontageComponent>(InOtherCharacter);
	montage->SetHittedAndDeadType(hittedType, deadType);

	TSubclassOf<UMatineeCameraShake> shake = data.ShakeClass;
	if (shake != NULL)
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->StartCameraShake(shake);

	FDamageEvent e;
	InOtherCharacter->TakeDamage(power, e, OwnerCharacter->GetController(), this);
}

void ACDoAction_Archer_Enemy::OnThrowBeginOverlap(FHitResult InHitResult, ACharacter* InOtherCharacter)
{
	CheckNull(InHitResult.GetActor());
	CheckNull(InOtherCharacter);

	const FDoActionData& data = DoActionDatas[0];

	float power = data.Power;

	TArray<UParticleSystem*> hitEffects = data.HitEffects;
	for (UParticleSystem* hitEffect : hitEffects)
	{
		if (hitEffect != NULL)
		{
			FTransform transform = data.HitEffectTransform;
			transform.AddToTranslation(InOtherCharacter->GetActorLocation());
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, transform);
		}
	}

	if (data.HitNiagaraEffect != NULL)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation
		(
			GetWorld(),
			data.HitNiagaraEffect,
			InOtherCharacter->GetActorLocation(),
			InOtherCharacter->GetActorRotation()
		);
	}

	EHittedType hittedType = data.HittedType;
	EDeadType deadType = data.DeadType;
	UCMontageComponent* montage = CHelpers::GetComponent<UCMontageComponent>(InOtherCharacter);
	montage->SetHittedAndDeadType(hittedType, deadType);

	TSubclassOf<UMatineeCameraShake> shake = data.ShakeClass;
	if (shake != NULL)
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->StartCameraShake(shake);

	FDamageEvent e;
	InOtherCharacter->TakeDamage(power, e, OwnerCharacter->GetController(), this);
}

void ACDoAction_Archer_Enemy::HitStopRestore()
{
	Super::HitStopRestore();

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);
}
