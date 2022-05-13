#include "Actions/CDoAction_Archer.h"
#include "Global.h"

#include "Actions/CAction.h"
#include "Actions/CAim.h"
#include "Actions/CThrow.h"
#include "Actions/CFake.h"
#include "Actions/CAttachment.h"
#include "Actions/CSkillEffect.h"

#include "Actors/CCreateDecal.h"
#include "Actors/CCreateCollisionBox.h"
#include "Actors/CRange.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CActionComponent.h"

#include "Camera/CameraComponent.h"

#include "Components/DecalComponent.h"

#include "Particles/ParticleSystemComponent.h"

#include "Components/SphereComponent.h"

#include "Characters/CPlayer.h"

ACDoAction_Archer::ACDoAction_Archer()
{
	Aim = NewObject<UCAim>();
}

void ACDoAction_Archer::BeginPlay()
{
	Super::BeginPlay();
	
	Aim->BeginPlay(OwnerCharacter);

	FTransform fakeArrowTransform;
	FActorSpawnParameters params;
	params.Owner = OwnerCharacter;

	const FDoActionData& data = DoActionDatas[0];

	// 페이크 화살 생성 
	if (data.FakeClass != NULL)
	{
		FakeArrow = GetWorld()->SpawnActorDeferred<ACFake>(data.FakeClass, fakeArrowTransform, OwnerCharacter, NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		UGameplayStatics::FinishSpawningActor(FakeArrow, fakeArrowTransform);

		FakeArrow->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), "ArrowSocket");
		FakeArrow->OnDisappear();
	}

	const FSkillActionData& skillData = SkillActionDatas[0];

	FTransform decalTransform; // 데칼 생성 위치
	FTransform skillRangeTransform; // 범위 생성 위치 

	// 데칼 생성
	if (skillData.DecalClass != NULL)
	{
		Decal = GetWorld()->SpawnActorDeferred<ACCreateDecal>(skillData.DecalClass, decalTransform, OwnerCharacter, NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		Decal->SetDisappear();
		Decal->OffWarning();
		Decal->SetIsArcherDecal(true);
		UGameplayStatics::FinishSpawningActor(Decal, decalTransform);
	}
	// 범위 샘성
	if (skillData.RangeClass != NULL)
	{
		SkillRange = GetWorld()->SpawnActorDeferred<ACRange>(skillData.RangeClass, skillRangeTransform, OwnerCharacter, NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		SkillRange->SetDisappear();
		SkillRange->OffWarning();
		SkillRange->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), "root");
		SkillRange->OnRangeEndOverlap.AddDynamic(this, &ACDoAction_Archer::OnRangeEndOverlap);
		SkillRange->OnRangeBeginOverlap.AddDynamic(this, &ACDoAction_Archer::OnRangeBeginOverlap);

		UGameplayStatics::FinishSpawningActor(SkillRange, skillRangeTransform);
	}

	bAirSwap = false;
}

void ACDoAction_Archer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckNull(Action);
	CheckNull(Aim);
	CheckFalse(Action->IsArcherMode());
	CheckNull(State);
	CheckTrue(State->IsStep());
	CheckTrue(State->IsEquip());

	Aim->ForwardTrace();

	if (Cast<ACPlayer>(OwnerCharacter) != NULL && Cast<ACPlayer>(OwnerCharacter)->GetAirSwap() == true && bAirSwap == false)
	{
		FVector direction = OwnerCharacter->GetActorUpVector();
		float power = 500.0f;
		OwnerCharacter->LaunchCharacter(direction * power, false, true);

		bAirSwap = true;
		
		bAirZoom = true;
		bAirShot = true;
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.3f);
	}

	// 마우스 커서 위치에 데칼 생성
	if(bReady == true) // 스킬 공격의 준비를 했다면
	{
		FVector location;
		FRotator rotator;

		if (GetCursorLocationAndRotation(location, rotator) == true)
		{
			location.Z += 15.0f;
			Decal->SetActorLocation(location);
		}
	}
}

void ACDoAction_Archer::OnAim()
{
	CheckNull(State);
	CheckTrue(State->IsSpecialAction());
	CheckTrue(State->IsSkillAction());
	CheckTrue(State->IsStep());
	CheckTrue(State->IsEquip());
	CheckNull(Action);
	CheckFalse(Action->IsArcherMode());
	CheckFalse(Aim->IsAvaliable());

	Type = EArcherStateType::OverDraw;
	const FDoActionData& data = DoActionDatas[(int32)Type];

	if (bAirZoom == true)
		OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio * 10.0f, data.StartSection); // 공중 상태에선 애니메이션 재생 속도 증가
	else
		OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);
	
	Aim->OnAim();
	State->SetAimAndOverdrawMode();
	
	UKismetSystemLibrary::K2_SetTimer(this, "ShowFakeArrow", 0.33f, false);
}

void ACDoAction_Archer::OffAim()
{
	CheckFalse(Aim->IsAvaliable());
	CheckFalse(Action->IsArcherMode());

	Aim->OffAim();
	FakeArrow->SetDisappear();
	FakeArrow->OnDisappear();
	
	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(OwnerCharacter);
	CheckNull(action);
	action->GetCurrent()->GetAttachment()->OnBowIdle();

	// 공중 상태에서 Aim일 경우 
	if (bAirZoom == true)
	{
		FVector direction = OwnerCharacter->GetActorUpVector() * -1.0f;
		float launchValue = 500.0f;
		OwnerCharacter->LaunchCharacter(direction * launchValue, false, true);
		UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
		OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
		
		if (Cast<ACPlayer>(OwnerCharacter) != NULL)
			Cast<ACPlayer>(OwnerCharacter)->DisableAirSwap();

		bAirZoom = false;
		bAirSwap = false;

		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	}

	State->SetIdleMode();
}

void ACDoAction_Archer::DoAction()
{
	Super::DoAction();
	CheckFalse(Action->IsArcherMode());

	if(State->IsSkillAction() == true) // 일반 공격이 아닌 스킬 공격일 경우 
	{
		Begin_SkillAction();
		return;
	}
	
	CheckTrue(State->IsAction());
	CheckFalse(IsFire);

	if(Aim->IsAvaliable() == true)
		CheckFalse(Aim->GetInZoom() == true);

	if (IsSpecialAction == true)
		IsSpecialAction = false;

	State->SetActionMode();

	Type = EArcherStateType::Fire;
	const FDoActionData& data = DoActionDatas[(int32)Type];
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);

	if (data.bCanMove == true)
		Status->SetMove();
	else
		Status->SetStop();
}

void ACDoAction_Archer::Begin_DoAction()
{
	Super::Begin_DoAction();
	CheckFalse(Action->IsArcherMode());

	if(State->IsTakedownAction() == true) // 테이크 다운 공격일 경우 
	{
		TakedownShotArrow();
		return;
	}

	// 화살의 스폰 위치
	FVector location = OwnerCharacter->GetMesh()->GetSocketLocation("ArrowSocket");
	// 화살의 스폰 방향 
	UCameraComponent* camera = CHelpers::GetComponent<UCameraComponent>(OwnerCharacter);
	FRotator rotator = camera->GetComponentRotation();
	rotator.Yaw += 1.15f;
	rotator.Pitch += 1.0f;
	
	FTransform transform;
	transform.AddToTranslation(location);
	transform.SetRotation(FQuat(rotator));

	FActorSpawnParameters params;
	params.Owner = OwnerCharacter;

	Type = EArcherStateType::Fire;
	const FDoActionData& data = DoActionDatas[(int32)Type];

	// 화살 생성
	ACThrow* throwObject = GetWorld()->SpawnActorDeferred<ACThrow>(data.ThrowClass, transform, OwnerCharacter, NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	throwObject->OnThrowBeginOverlap.AddDynamic(this, &ACDoAction_Archer::OnThrowBeginOverlap);
	throwObject->SetIsArcher_Takedown(false);
	throwObject->SetIsArcher_SpecialAction(false);
	throwObject->SetIsArcher(true);
	UGameplayStatics::FinishSpawningActor(throwObject, transform);

	// 페이크 화살 Off
	FakeArrow->SetDisappear();
	FakeArrow->OnDisappear();
}

void ACDoAction_Archer::End_DoAction()
{
	Super::End_DoAction();

	CheckTrue(State->IsTakedownAction() == true);

	Type = EArcherStateType::Fire;
	const FDoActionData& prevData = DoActionDatas[(int32)Type];
	OwnerCharacter->StopAnimMontage(prevData.AnimMontage);

	if(Aim->GetInZoom() == true)
	{
		Type = EArcherStateType::OverDraw;
		const FDoActionData& data = DoActionDatas[(int32)Type];
		OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);
		UKismetSystemLibrary::K2_SetTimer(this, "ShowFakeArrow", 0.3f, false);
	}

	if (bAirShot == true)
	{
		OffAim();
		bAirShot = false;
	}

	State->SetIdleMode();
	Status->SetMove();
	IsFire = false;

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);

	HitCharacters.Empty();
}

//  모든 상태값 리셋 
void ACDoAction_Archer::Reset()
{
	Super::Reset();

	OffAim();
	Status->SetMove();
}

void ACDoAction_Archer::SkillAction()
{
	Super::SkillAction();
	CheckFalse(Action->IsArcherMode());

	CheckFalse(State->IsIdle());
	CheckTrue(Aim->GetInZoom());
	
	//CLog::Print("SkillAction");
	State->SetSkillActionMode();

	// 상태값, 페이크 화살, 범위 셋팅
	bReady = true;

	FakeArrow->SetSpecialAppear();
	FakeArrow->OnDisappear();

	SkillRange->SetAppear();
	Decal->SetAppear();

	// 충돌체 켜기
	Decal->GetSphere()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SkillRange->GetSphere()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACDoAction_Archer::Begin_SkillAction()
{
	Super::Begin_SkillAction();
	CheckFalse(Action->IsArcherMode());

	CheckFalse(State->IsSkillAction());
	CheckTrue(State->IsHitted() == true);
	
	FRotator rotation;
	FVector location;
	bool result = GetCursorLocationAndRotation(location, rotation); // 마우스 커서의 위치와 방향값 가져오기 
	if (result == false)
		return;

	if (bWarning == true) // 스킬 범위를 벗어나면
	{
		const FSkillActionData& data = SkillActionDatas[0];

		TSubclassOf<UMatineeCameraShake> shake = data.ShakeClass;
		if (shake != NULL) // 카메라 쉐이크 켜기 
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->StartCameraShake(shake);

		return;
	}

	Decal->SetActorLocation(location);

	HitLocation = location;
	
	const FSkillActionData& data = SkillActionDatas[0];
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection); // 애니메이션 실행
	bReady = false;

	// 스킬 공격을 했다면 데칼과 범위 충돌체 끄기
	Decal->GetSphere()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SkillRange->GetSphere()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 값 셋팅 
	Decal->SetDisappear();
	Decal->OffWarning();
	SkillRange->SetDisappear();
	SkillRange->OffWarning();

	FakeArrow->SetSpecialDisAppear();

	if (data.bCanMove == true)
		Status->SetMove();
	else
		Status->SetStop();
}

void ACDoAction_Archer::End_SkillAction()
{
	Super::End_SkillAction();

	State->SetIdleMode();
	Status->SetMove();

	const FSkillActionData& data = SkillActionDatas[0];
	OwnerCharacter->StopAnimMontage(data.AnimMontage);

	for (ACCreateCollisionBox* collisionBox : CollisionBoxs)
		collisionBox->Destroy();

	CollisionBoxs.Empty();
	HitCharacters.Empty();

	OwnerCharacter->bUseControllerRotationYaw = true;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;

}

void ACDoAction_Archer::SpecialAction()
{
	Super::SpecialAction();
	CheckFalse(Action->IsArcherMode());
	CheckFalse(State->IsIdle() == true);

	State->SetSpecialActionMode();

	const FSpecialActionData& data = SpecialActionDatas[0];
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);

	IsSpecialAction = true;
}

void ACDoAction_Archer::Begin_SpecialAction()
{
	Super::Begin_SpecialAction();
	CheckFalse(Action->IsArcherMode());
	
	CheckFalse(State->IsSpecialAction());

	const FSpecialActionData& data = SpecialActionDatas[0];

	// 페이크 화살 생성 
	FTransform fakeArrowTransform;
	FActorSpawnParameters params;
	params.Owner = OwnerCharacter;

	FakeArrow_Special = GetWorld()->SpawnActorDeferred<ACFake>(data.FakeClass, fakeArrowTransform, OwnerCharacter, NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	FakeArrow_Special->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), "ArrowSocket");
	
	UGameplayStatics::FinishSpawningActor(FakeArrow_Special, fakeArrowTransform);
}

void ACDoAction_Archer::End_SpecialAction()
{
	Super::End_SpecialAction();

	State->SetIdleMode();
	IsSpecialAction = false;

	HitCharacters.Empty();
}
// 스킬 Effect 생성
void ACDoAction_Archer::CreateSkillEffect()
{
	const FSpecialActionData& data = SpecialActionDatas[0];

	FTransform transform;
	FActorSpawnParameters params;
	params.Owner = OwnerCharacter;

	FVector location = OwnerCharacter->GetActorLocation();
	FVector direction = OwnerCharacter->GetActorForwardVector();
	location += direction * 300.0f;
	transform.AddToTranslation(location);
	transform.SetRotation(FQuat(OwnerCharacter->GetActorRotation()));

	SpecialEffect = GetWorld()->SpawnActor<ACSkillEffect>(data.SkillEffectClass, transform, params);
	SpecialEffect->SizeUpEffect();
}
// 24개의 화살을 발사
void ACDoAction_Archer::ArcherSpecial_ShotArrow()
{
	//CLog::Log("ArcherSpecial_ShotArrow");

	FakeArrow_Special->Destroy();

	FRotator throwRotator = OwnerCharacter->GetActorForwardVector().Rotation();
	ThrowRotators.Add(throwRotator);

	// 화살의 방향 값 셋팅 
	for (int i = 0; i < 12; i++)
	{
		FRotator r = throwRotator;
		r.Yaw -= i * 5.0f;

		ThrowRotators.Add(r);
	}

	throwRotator.Yaw += 5.0f;

	for (int i = 0; i < 12; i++)
	{
		FRotator r = throwRotator;
		r.Yaw += i * 5.0f;

		ThrowRotators.Add(r);
	}

	const FSpecialActionData& data = SpecialActionDatas[0];

	// 화살 생성 
	for (FRotator r : ThrowRotators)
	{
		FTransform transform;
		FVector location = SpecialEffect->GetActorLocation();
		transform.AddToTranslation(location);
		FRotator rotator = r;
		transform.SetRotation(FQuat(rotator));

		FActorSpawnParameters params;
		params.Owner = OwnerCharacter;
	
		ACThrow* throw_Special = GetWorld()->SpawnActorDeferred<ACThrow>(data.ThrowClass, transform, OwnerCharacter, NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		throw_Special->OnThrowSpecialBeginOverlap.AddDynamic(this, &ACDoAction_Archer::OnThrowSpecialBeginOverlap);
		throw_Special->SetIsArcher_Takedown(false);
		throw_Special->SetIsArcher_SpecialAction(true);
		UGameplayStatics::FinishSpawningActor(throw_Special, transform);
	}

	SpecialEffect->Destroy();

	TSubclassOf<UMatineeCameraShake> shake = data.ShakeClass;
	if (shake != NULL)
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->StartCameraShake(shake);
}

void ACDoAction_Archer::TakedownAction()
{
	Super::TakedownAction();
	CheckTrue(State->IsTakedownAction());
	CheckFalse(IsFire);
	CheckTrue(State->IsHitted() == true);

	if (Aim->IsAvaliable() == true)
		CheckFalse(Aim->GetInZoom() == true);

	State->SetTakedownActionMode();

	Type = EArcherStateType::Fire;
	const FDoActionData& data = DoActionDatas[(int32)Type];
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);
	//UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.1f);

	if (data.bCanMove == true)
		Status->SetMove();
	else
		Status->SetStop();
}

// 테이크 다운 화살 발사 
void ACDoAction_Archer::TakedownShotArrow()
{
	FVector location = OwnerCharacter->GetMesh()->GetSocketLocation("ArrowSocket");
	UCameraComponent* camera = CHelpers::GetComponent<UCameraComponent>(OwnerCharacter);
	FRotator rotator = camera->GetComponentRotation();
	rotator.Yaw += 2.15f;

	FTransform transform;
	transform.AddToTranslation(location);
	transform.SetRotation(FQuat(rotator));

	FActorSpawnParameters params;
	params.Owner = OwnerCharacter;

	Type = EArcherStateType::Fire;
	const FDoActionData& data = DoActionDatas[(int32)Type];

	ACThrow* throwObject = GetWorld()->SpawnActorDeferred<ACThrow>(data.ThrowClass, transform, OwnerCharacter, NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	throwObject->OnThrowBeginOverlap.AddDynamic(this, &ACDoAction_Archer::OnThrowBeginOverlap);
	throwObject->SetIsArcher_Takedown(true);
	throwObject->SetIsArcher_SpecialAction(false);
	UGameplayStatics::FinishSpawningActor(throwObject, transform);
	
	FakeArrow->SetDisappear();
	FakeArrow->OnDisappear();
}

void ACDoAction_Archer::OnThrowBeginOverlap(FHitResult InHitResult, ACharacter* InOtherCharacter)
{
	CheckNull(InHitResult.GetActor());
	CheckNull(InOtherCharacter);

	if(State->IsTakedownAction() == true)
	{
		WarpArrow_Archer(InOtherCharacter);
		return;
	}

	// 중복 데미지 방지 
	for (const ACharacter* hitCharacter : HitCharacters)
	{
		if (InOtherCharacter == hitCharacter)
			return;
	}

	HitCharacters.AddUnique(InOtherCharacter);
	//HitCharacters.Empty();

	Type = EArcherStateType::Fire;
	const FDoActionData& data = DoActionDatas[(int32)Type];
	float power = data.Power;
	DoActionReadData(data, InOtherCharacter);

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(InOtherCharacter);
	CheckNull(state);
	if (state->IsDead() == true) // 적이 죽었다면
		return;

	FDamageEvent e;
	InOtherCharacter->TakeDamage(power, e, OwnerCharacter->GetController(), this);

}

void ACDoAction_Archer::OnThrowSpecialBeginOverlap(FHitResult InHitResult, ACharacter* InOtherCharacter)
{
	CheckNull(InHitResult.GetActor());
	CheckNull(InOtherCharacter);

	for (const ACharacter* hitCharacter : HitCharacters)
	{
		if (InOtherCharacter == hitCharacter)
			return;
	}
	HitCharacters.Add(InOtherCharacter);
	//HitCharacters.Empty();

	const FSpecialActionData& data = SpecialActionDatas[0];
	SpecialActionReadData(data, InOtherCharacter);
	float power = data.Power;

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(InOtherCharacter);
	CheckNull(state);
	if (state->IsDead() == true)
		return;

	FDamageEvent e;
	InOtherCharacter->TakeDamage(power, e, OwnerCharacter->GetController(), this);
}

void ACDoAction_Archer::OnCollisionBoxBeginOverlap(FHitResult InHitResult, ACharacter* InOtherCharacter)
{
	CheckNull(InHitResult.GetActor());
	CheckNull(InOtherCharacter);

	for (const ACharacter* hitCharacter : HitCharacters)
	{
		if (InOtherCharacter == hitCharacter)
			return;
	}
	HitCharacters.Add(InOtherCharacter);
	//HitCharacters.Empty();

	// 데이터를 가져와서 값 셋팅
	const FSkillActionData& data = SkillActionDatas[0];

	// 공격력
	float power = data.Power; 

	// 파티클
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
	// 파티클
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
	// 상태값 
	EHittedType hittedType = data.HittedType;
	EDeadType deadType = data.DeadType;
	UCMontageComponent* montage = CHelpers::GetComponent<UCMontageComponent>(InOtherCharacter);
	montage->SetHittedAndDeadType(hittedType, deadType);

	TSubclassOf<UMatineeCameraShake> shake = data.ShakeClass;
	if (shake != NULL)
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->StartCameraShake(shake);

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(InOtherCharacter);
	CheckNull(state);
	if (state->IsDead() == true)
		return;

	FDamageEvent e;
	InOtherCharacter->TakeDamage(power, e, OwnerCharacter->GetController(), this);
}
// DoAction 데이터 읽기 
void ACDoAction_Archer::DoActionReadData(const FDoActionData& InData, class ACharacter* InOtherCharacter)
{
	TArray<UParticleSystem*> hitEffects = InData.HitEffects;
	for (UParticleSystem* hitEffect : hitEffects)
	{
		if (hitEffect != NULL)
		{
			FTransform transform = InData.HitEffectTransform;
			transform.AddToTranslation(InOtherCharacter->GetActorLocation());
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, transform);
		}
	}

	if (InData.HitNiagaraEffect != NULL)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation
		(
			GetWorld(),
			InData.HitNiagaraEffect,
			InOtherCharacter->GetActorLocation(),
			InOtherCharacter->GetActorRotation()
		);
	}

	EHittedType hittedType = InData.HittedType;
	EDeadType deadType = InData.DeadType;
	UCMontageComponent* montage = CHelpers::GetComponent<UCMontageComponent>(InOtherCharacter);
	montage->SetHittedAndDeadType(hittedType, deadType);

	TSubclassOf<UMatineeCameraShake> shake = InData.ShakeClass;
	if (shake != NULL)
	{
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->StartCameraShake(shake);
	}
}

// SpecialAction 데이터 읽기
void ACDoAction_Archer::SpecialActionReadData(const FSpecialActionData& InData, class ACharacter* InOtherCharacter)
{
	TArray<UParticleSystem*> hitEffects = InData.HitEffects;
	for (UParticleSystem* hitEffect : hitEffects)
	{
		if (hitEffect != NULL)
		{
			FTransform transform = InData.HitEffectTransform;
			transform.AddToTranslation(InOtherCharacter->GetActorLocation());
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, transform);
		}
	}

	if (InData.NiagaraEffect != NULL)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation
		(
			GetWorld(),
			InData.NiagaraEffect,
			InOtherCharacter->GetActorLocation(),
			InOtherCharacter->GetActorRotation()
		);
	}

	EHittedType hittedType = InData.HittedType;
	EDeadType deadType = InData.DeadType;
	UCMontageComponent* montage = CHelpers::GetComponent<UCMontageComponent>(InOtherCharacter);
	montage->SetHittedAndDeadType(hittedType, deadType);

}

void ACDoAction_Archer::ShowFakeArrow()
{
	if(Aim->GetInZoom() == true)
	{
		FakeArrow->SetAppear();
		FakeArrow->OnAppear();
	}
}

void ACDoAction_Archer::AirJump()
{
	FVector direction = OwnerCharacter->GetActorUpVector();
	const FAirActionData& data = AirActionDatas[0];
	float value = data.AirLaunchMeValue;
	OwnerCharacter->LaunchCharacter(direction * value, false, true);
	State->SetFlyingMode();
	OwnerCharacter->StopAnimMontage(data.AnimMontage);
}
// 워프 화살에 맞을 경우 위치 이동  
void ACDoAction_Archer::WarpArrow_Archer(ACharacter* InOther)
{
	//CLog::Print(InOther->GetName());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(OwnerCharacter);
	CheckNull(action);
	action->GetCurrent()->GetAttachment()->OnAppear();
	
	OwnerCharacter->GetCharacterMovement()->DisableMovement();
	
	VictimCharacter = InOther;
	FVector location = VictimCharacter->GetActorLocation() + (VictimCharacter->GetActorForwardVector() * (-125.0f));
	OwnerCharacter->SetActorLocation(location);
	OffAim();
	
	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
	
	OwnerCharacter->SetActorRotation(VictimCharacter->GetActorRotation());
	
	const FTakedownActionData& data = TakedownActionDatas[0];
	float attDelay = OwnerCharacter->PlayAnimMontage(data.AttAnimMontage, data.AttPlayRatio, data.AttStartSection);
	
	EEnemyType enemyType = Cast<ACEnemy>(VictimCharacter)->GetEnemyType();
	switch (enemyType) // 적에 종류에 따라 애니메이션 별도 실행
	{
		case EEnemyType::Dummy:
		{
			bool bHas = data.VicMaps.Contains(EEnemyType::Dummy);
			if (bHas == true)
			{
				UAnimMontage* animMontage = data.VicMaps[EEnemyType::Dummy];
				VictimCharacter->PlayAnimMontage(animMontage, data.VicPlayRatio, data.VicStartSection);
			}
		}
			break;

		case EEnemyType::Knight:
		{
			bool bHas = data.VicMaps.Contains(EEnemyType::Knight);
			if (bHas == true)
			{
				UAnimMontage* animMontage = data.VicMaps[EEnemyType::Knight];
				VictimCharacter->PlayAnimMontage(animMontage, data.VicPlayRatio, data.VicStartSection);
			}
		}
			break;

		case EEnemyType::GirlKnight:
		{
			bool bHas = data.VicMaps.Contains(EEnemyType::GirlKnight);
			if (bHas == true)
			{
				UAnimMontage* animMontage = data.VicMaps[EEnemyType::GirlKnight];
				VictimCharacter->PlayAnimMontage(animMontage, data.VicPlayRatio, data.VicStartSection);
			}
		}
			break;

		default:
			break;
	}

	//UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.5f);

	UKismetSystemLibrary::K2_SetTimer(this, "TakedownFinish", attDelay, false);

	UCMontageComponent* vicMontage = CHelpers::GetComponent<UCMontageComponent>(VictimCharacter);
	CheckNull(vicMontage);

	EHittedType hittedType = EHittedType::Max;
	EDeadType deadType = EDeadType::Takedown;
	vicMontage->SetHittedAndDeadType(hittedType, deadType);

	FDamageEvent e;
	VictimCharacter->TakeDamage(0.0f, e, OwnerCharacter->GetController(), this);
}

void ACDoAction_Archer::TakedownFinish()
{
	//CLog::Log("TakedownFinish");
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);

	FVector location = OwnerCharacter->GetActorLocation();

	OwnerCharacter->SetActorLocation(location);

	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	VictimCharacter = NULL;

	OwnerCharacter->bUseControllerRotationYaw = true;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(OwnerCharacter);
	CheckNull(action);
	action->GetCurrent()->GetAttachment()->OnDisappear();

	State->SetIdleMode();
	Status->SetMove();
}

void ACDoAction_Archer::ArcherSkill_SpawnHitEffect()
{
	//CLog::Print("ArcherSkill_SpawnHitEffect");
	
	const FSkillActionData& data = SkillActionDatas[0];
	FTransform transform;
	//transform.SetLocation(HitLocation);
	transform.AddToTranslation(HitLocation);

	ACCreateCollisionBox* collisionBox = GetWorld()->SpawnActorDeferred<ACCreateCollisionBox>(SkillActionDatas[0].CollisionBoxClass, transform, OwnerCharacter, NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	collisionBox->OnCollisionBoxBeginOverlap.AddDynamic(this, &ACDoAction_Archer::OnCollisionBoxBeginOverlap);
	
	UGameplayStatics::FinishSpawningActor(collisionBox, transform);

	CollisionBoxs.Add(collisionBox);
}
// 마우스 커서의 위치와 방향 가져오기
bool ACDoAction_Archer::GetCursorLocationAndRotation(FVector& OutLocation, FRotator& OutRotator)
{
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	TArray<TEnumAsByte<EObjectTypeQuery>> objects;
	objects.Add(EObjectTypeQuery::ObjectTypeQuery1);

	FHitResult hitResult;
	if (controller->GetHitResultUnderCursorForObjects(objects, false, hitResult))
	{
		OutLocation = hitResult.Location;
		OutRotator = hitResult.ImpactNormal.Rotation();

		return true;
	}

	return false;
}

void ACDoAction_Archer::ArcherSkill_SpawnShotEffect()
{
	//CLog::Print("ArcherSkill_SpawnShotEffect");

	const FSkillActionData& data = SkillActionDatas[0];

	FVector location = OwnerCharacter->GetMesh()->GetSocketLocation("hand_l");
	FRotator rotation = OwnerCharacter->GetActorForwardVector().Rotation();
	rotation.Pitch += 45.0f;
	
	// 0번 index가 ShotEffect
	TArray<UParticleSystem*> shotEffects = data.ShotEffects;
	if (shotEffects[0] != NULL)
	{
		FTransform transform = data.ShotEffectTransform;
		transform.AddToTranslation(location);
		transform.SetRotation(FQuat(rotation));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), shotEffects[0], transform);
	}

	UKismetSystemLibrary::K2_SetTimer(this, "ArcherSkill_SpawnHitEffect", 0.3f, false);
}

void ACDoAction_Archer::OnActionTypeChanged(EActionType InPrevType, EActionType InNewType)
{
	if (InPrevType == EActionType::Dual && InNewType == EActionType::Archer)
	{
		//if(OwnerCharacter->GetCharacterMovement()->Velocity.Z < 0.0f) // 낙하
		//{
		//	bAirZoom = true;
		//	bAirShot = true;

		//	OnAim();
		//}
	}
}

void ACDoAction_Archer::OnRangeBeginOverlap(AActor* InOtherActor)
{
	CheckNull(InOtherActor);

	CheckFalse(Action->IsArcherMode());
	CheckFalse(bReady);

	bWarning = false;
	Decal->OffWarning();
	Decal->SetAppear();

	SkillRange->OffWarning();
	SkillRange->SetAppear();
}

void ACDoAction_Archer::OnRangeEndOverlap(AActor* InOtherActor)
{
	CheckNull(InOtherActor);

	CheckFalse(Action->IsArcherMode());
	CheckFalse(bReady);

	bWarning = true;
	Decal->OnWarning();
	Decal->SetDisappear();

	SkillRange->OnWarning();
	SkillRange->SetDisappear();
}
