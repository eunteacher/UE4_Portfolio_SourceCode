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

	// 其捞农 拳混 积己 
	if (data.FakeClass != NULL)
	{
		FakeArrow = GetWorld()->SpawnActorDeferred<ACFake>(data.FakeClass, fakeArrowTransform, OwnerCharacter, NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		UGameplayStatics::FinishSpawningActor(FakeArrow, fakeArrowTransform);

		FakeArrow->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), "ArrowSocket");
		FakeArrow->OnDisappear();
	}

	const FSkillActionData& skillData = SkillActionDatas[0];

	FTransform decalTransform; // 单漠 积己 困摹
	FTransform skillRangeTransform; // 裹困 积己 困摹 

	// 单漠 积己
	if (skillData.DecalClass != NULL)
	{
		Decal = GetWorld()->SpawnActorDeferred<ACCreateDecal>(skillData.DecalClass, decalTransform, OwnerCharacter, NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		Decal->SetDisappear();
		Decal->OffWarning();
		Decal->SetIsArcherDecal(true);
		UGameplayStatics::FinishSpawningActor(Decal, decalTransform);
	}
	// 裹困 基己
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

	// 付快胶 目辑 困摹俊 单漠 积己
	if(bReady == true) // 胶懦 傍拜狼 霖厚甫 沁促搁
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
		OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio * 10.0f, data.StartSection); // 傍吝 惑怕俊急 局聪皋捞记 犁积 加档 刘啊
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

	// 傍吝 惑怕俊辑 Aim老 版快 
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

	if(State->IsSkillAction() == true) // 老馆 傍拜捞 酒囱 胶懦 傍拜老 版快 
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

	if(State->IsTakedownAction() == true) // 抛捞农 促款 傍拜老 版快 
	{
		TakedownShotArrow();
		return;
	}

	// 拳混狼 胶迄 困摹
	FVector location = OwnerCharacter->GetMesh()->GetSocketLocation("ArrowSocket");
	// 拳混狼 胶迄 规氢 
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

	// 拳混 积己
	ACThrow* throwObject = GetWorld()->SpawnActorDeferred<ACThrow>(data.ThrowClass, transform, OwnerCharacter, NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	throwObject->OnThrowBeginOverlap.AddDynamic(this, &ACDoAction_Archer::OnThrowBeginOverlap);
	throwObject->SetIsArcher_Takedown(false);
	throwObject->SetIsArcher_SpecialAction(false);
	throwObject->SetIsArcher(true);
	UGameplayStatics::FinishSpawningActor(throwObject, transform);

	// 其捞农 拳混 Off
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

//  葛电 惑怕蔼 府悸 
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

	// 惑怕蔼, 其捞农 拳混, 裹困 悸泼
	bReady = true;

	FakeArrow->SetSpecialAppear();
	FakeArrow->OnDisappear();

	SkillRange->SetAppear();
	Decal->SetAppear();

	// 面倒眉 难扁
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
	bool result = GetCursorLocationAndRotation(location, rotation); // 付快胶 目辑狼 困摹客 规氢蔼 啊廉坷扁 
	if (result == false)
		return;

	if (bWarning == true) // 胶懦 裹困甫 哈绢唱搁
	{
		const FSkillActionData& data = SkillActionDatas[0];

		TSubclassOf<UMatineeCameraShake> shake = data.ShakeClass;
		if (shake != NULL) // 墨皋扼 溅捞农 难扁 
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->StartCameraShake(shake);

		return;
	}

	Decal->SetActorLocation(location);

	HitLocation = location;
	
	const FSkillActionData& data = SkillActionDatas[0];
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection); // 局聪皋捞记 角青
	bReady = false;

	// 胶懦 傍拜阑 沁促搁 单漠苞 裹困 面倒眉 掺扁
	Decal->GetSphere()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SkillRange->GetSphere()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 蔼 悸泼 
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

	// 其捞农 拳混 积己 
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
// 胶懦 Effect 积己
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
// 24俺狼 拳混阑 惯荤
void ACDoAction_Archer::ArcherSpecial_ShotArrow()
{
	//CLog::Log("ArcherSpecial_ShotArrow");

	FakeArrow_Special->Destroy();

	FRotator throwRotator = OwnerCharacter->GetActorForwardVector().Rotation();
	ThrowRotators.Add(throwRotator);

	// 拳混狼 规氢 蔼 悸泼 
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

	// 拳混 积己 
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

// 抛捞农 促款 拳混 惯荤 
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

	// 吝汗 单固瘤 规瘤 
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
	if (state->IsDead() == true) // 利捞 磷菌促搁
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

	// 单捞磐甫 啊廉客辑 蔼 悸泼
	const FSkillActionData& data = SkillActionDatas[0];

	// 傍拜仿
	float power = data.Power; 

	// 颇萍努
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
	// 颇萍努
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
	// 惑怕蔼 
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
// DoAction 单捞磐 佬扁 
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

// SpecialAction 单捞磐 佬扁
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
// 况橇 拳混俊 嘎阑 版快 困摹 捞悼  
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
	switch (enemyType) // 利俊 辆幅俊 蝶扼 局聪皋捞记 喊档 角青
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
// 付快胶 目辑狼 困摹客 规氢 啊廉坷扁
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
	
	// 0锅 index啊 ShotEffect
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
		//if(OwnerCharacter->GetCharacterMovement()->Velocity.Z < 0.0f) // 倡窍
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
