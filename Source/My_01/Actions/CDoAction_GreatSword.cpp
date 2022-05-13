#include "Actions/CDoAction_GreatSword.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"

#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CMontageComponent.h"
#include "Components/CBehaviorCompoent.h"

#include "Actors/CCreateDecal.h"
#include "Actors/CCannon.h"
#include "Actors/CCannonTarget.h"
#include "Actors/CSummonController.h"

#include "Actions/CSkillEffect.h"
#include "Actions/CThrow.h"

ACDoAction_GreatSword::ACDoAction_GreatSword()
{

}

void ACDoAction_GreatSword::DoAction()
{
	Super::DoAction();
	CheckNull(State);
	CheckFalse(DoActionDatas.Num() > 0);
	CheckFalse(State->IsIdle());
	CheckFalse(bDoAction);
	CheckTrue(IsSkillAction);
	CheckTrue(IsAirAction);
	CheckTrue(IsSpecialAction);
	CheckTrue(IsSummonAction);

	State->SetActionMode();

	const FDoActionData& data = DoActionDatas[0];
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);

	if (data.bCanMove == true)
		Status->SetMove();
	else
		Status->SetStop();
}

void ACDoAction_GreatSword::Begin_DoAction()
{
	Super::Begin_DoAction();

	if (IsAirAction == true)
	{
		Begin_AirAction();
		return;
	}

	if (IsSkillAction == true)
	{
		Begin_SkillAction();
		return;
	}
	
	if (IsSpecialAction == true)
	{
		Begin_SpecialAction();
		return;
	}

	if (IsSummonAction == true)
	{
		BeginSummonAction();
		return;
	}

	CheckFalse(bBeginDoAction);

	bBeginDoAction = false;

	if (DoActionDatas.Num() < ComboIndex + 1)
		ComboIndex = 0;

	const FDoActionData& prevData = DoActionDatas[ComboIndex];
	OwnerCharacter->StopAnimMontage(prevData.AnimMontage);

	ComboIndex++;
	bDoAction = false;

	const FDoActionData& data = DoActionDatas[ComboIndex];
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);
}

void ACDoAction_GreatSword::End_DoAction()
{
	Super::End_DoAction();
	CheckNull(State);
	CheckNull(Status);

	if (IsSkillAction == true)
	{
		End_SkillAction();
		return;
	}

	if (IsAirAction == true)
	{
		End_AirAction();
		return;
	}
	
	if (IsSpecialAction == true)
	{
		End_SpecialAction();
		return;
	}

	if (IsSummonAction == true)
	{
		EndSummonAction();
		return;
	}

	State->SetIdleMode();
	Status->SetMove();
	ComboIndex = 0;
	bDoAction = true;;
	bBeginDoAction = true;
	HitCharacters.Empty();

	IsDoAction = true;
	IsAirAction = false;
	IsSkillAction = false;
	IsSpecialAction = false;
	IsSummonAction = false;
}

void ACDoAction_GreatSword::AirAction()
{
	Super::AirAction();
	CheckNull(State);
	CheckFalse(AirActionDatas.Num() > 0);
	CheckFalse(State->IsIdle());
	CheckFalse(bAirAction);
	CheckTrue(IsSkillAction);
	CheckTrue(IsSpecialAction);
	CheckTrue(IsSummonAction);

	IsDoAction = false;
	IsAirAction = true;
	IsSkillAction = false;
	IsSpecialAction = false;

	State->SetAirActionMode();

	const FAirActionData& data = AirActionDatas[0];
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);

	if (data.bCanMove == true)
		Status->SetMove();
	else
		Status->SetStop();
}

void ACDoAction_GreatSword::Begin_AirAction()
{
	Super::Begin_AirAction();

	CheckFalse(bBeginAirAction);

	bBeginAirAction = false;

	if (AirActionDatas.Num() < AirComboIndex + 1)
		AirComboIndex = 0;

	const FAirActionData& prevData = AirActionDatas[AirComboIndex];
	OwnerCharacter->StopAnimMontage(prevData.AnimMontage);

	AirComboIndex++;
	//CLog::Log(AirComboIndex);
	bAirAction = false;

	const FAirActionData& data = AirActionDatas[AirComboIndex];
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);
}

void ACDoAction_GreatSword::End_AirAction()
{
	Super::End_AirAction();
	CheckNull(State);
	CheckNull(Status);

	State->SetIdleMode();
	Status->SetMove();
	AirComboIndex = 0;
	bAirAction = true;
	bBeginAirAction = true;
	HitCharacters.Empty();

	IsDoAction = true;
	IsAirAction = false;
	IsSkillAction = false;
	IsSpecialAction = false;
}

void ACDoAction_GreatSword::SkillAction()
{
	Super::SkillAction();
	CheckNull(State);
	CheckNull(Status);
	CheckFalse(State->IsIdle() == true); 

	IsDoAction = false;
	IsAirAction = false;
	IsSkillAction = true;
	IsSpecialAction = false;

	State->SetSkillActionMode();

	const FSkillActionData& data = SkillActionDatas[SkillIndex];
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);

	if (data.bCanMove == true)
		Status->SetMove();
	else
		Status->SetStop();

	switch (SkillIndex)
	{
		case 0:
		{
			// 1번째 스킬
			SetSpawnDecalLoaction();
			CheckTrue(DecalLocations.Num() < 0);
			CreateSkillDecal();
		}
			break;

		case 1:
		{
			SpawnGreatSword();
		}
			break;
		case 2:
		{
			// 3번째 스킬
			SetSpawnDecalLoaction();

			CheckTrue(DecalLocations.Num() < 0);
			CreateSkillDecal();
		}
			break;

		default:
			break;
	}

}

void ACDoAction_GreatSword::Begin_SkillAction()
{
	Super::Begin_SkillAction();
	CheckTrue(SkillEffectLocations.Num() < 0);

	CheckFalse(IsSkillAction);

	switch (SkillIndex)
	{
		case 0:
		{
			CreateSkillThrow();
			DestroySkillEffects();
		}
			break;
	
		case 1:
		{
			SwingGreatSword();

			GreatSword->OnNiagara_Slash();
			GreatSword->OnNiagara_GreatSword();
		}
			break;
	
		case 2:
		{
			FVector location = Decals[0]->GetActorLocation();
			FVector direction = OwnerCharacter->GetActorForwardVector();
			location -= direction * 300.0f;
			location.Z += 70.0f;

			OwnerCharacter->SetActorLocation(location);
		}
			break;

		default:
			break;
	}

	
}

void ACDoAction_GreatSword::End_SkillAction()
{
	Super::End_SkillAction();

	if (SkillIndex == 2)
		DestroySkillEffects();

	State->SetIdleMode();
	Status->SetMove();
	DecalLocations.Empty();
	SkillEffectLocations.Empty();
	Decals.Empty();
	SkillEffects.Empty();
	HitCharacters.Empty();
	
	IsDoAction = true;
	IsAirAction = false;
	IsSkillAction = false;
	IsSpecialAction = false;

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}

void ACDoAction_GreatSword::SpecialAction()
{
	Super::SpecialAction();
	CheckNull(State);
	CheckFalse(State->IsIdle() == true);

	IsDoAction = false;
	IsAirAction = false;
	IsSkillAction = false;
	IsSpecialAction = true;

	State->SetSpecialActionMode();

	SpecialActionIndex = UKismetMathLibrary::RandomIntegerInRange(0, 1);
	const FSpecialActionData& data = SpecialActionDatas[SpecialActionIndex];
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);

	if (data.bCanMove == true)
		Status->SetMove();
	else
		Status->SetStop();
}

void ACDoAction_GreatSword::Begin_SpecialAction()
{
	Super::Begin_SpecialAction();

	ACharacter* target = Cast<ACharacter>(Behavior->GetTargetPlayer());
	CheckNull(target);

	OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	switch (SpecialActionIndex)
	{
		case 0:
		{
			//CLog::Log("Begin_SpecialAction : Cast : 0");
			FLatentActionInfo info;
			info.CallbackTarget = this;
			info.UUID = GetUniqueID();
			info.ExecutionFunction = "Rolling_SpecialAction";
			info.Linkage = 0;

			float distance = UKismetMathLibrary::Vector_Distance(OwnerCharacter->GetActorLocation(), target->GetActorLocation());

			FVector location = OwnerCharacter->GetActorLocation() + (OwnerCharacter->GetActorForwardVector() * distance);
			location.Z += 250.0f;
		
			TargetLocation = OwnerCharacter->GetActorLocation() + (OwnerCharacter->GetActorForwardVector() * distance);

			FRotator rotation = OwnerCharacter->GetActorRotation();

			UKismetSystemLibrary::MoveComponentTo
			(
				OwnerCharacter->GetCapsuleComponent(),
				location,
				rotation,
				false,
				false,
				1.0f,
				false,
				EMoveComponentAction::Move,
				info
			);

		}
			break;

		case 1:
		{
			//CLog::Log("Begin_SpecialAction : Cast : 1");

			FLatentActionInfo info;
			info.CallbackTarget = this;
			info.UUID = GetUniqueID();
			info.ExecutionFunction = "ExecutionFunction_SkillAction";
			info.Linkage = 0;

			float distance = UKismetMathLibrary::Vector_Distance(OwnerCharacter->GetActorLocation(), target->GetActorLocation());

			FVector location = OwnerCharacter->GetActorLocation() + (OwnerCharacter->GetActorForwardVector() * distance);
			FRotator rotation = OwnerCharacter->GetActorRotation();

			UKismetSystemLibrary::MoveComponentTo
			(
				OwnerCharacter->GetCapsuleComponent(),
				location,
				rotation,
				false,
				false,
				1.2f,
				false,
				EMoveComponentAction::Move,
				info
			);
		}
			break;

		default:
			break;
	}
}

void ACDoAction_GreatSword::Rolling_SpecialAction()
{
	FLatentActionInfo info;
	info.CallbackTarget = this;
	info.UUID = GetUniqueID();
	info.ExecutionFunction = "ExecutionFunction_SkillAction";
	info.Linkage = 0;

	FVector location = TargetLocation;

	FRotator rotation = OwnerCharacter->GetActorRotation();

	UKismetSystemLibrary::MoveComponentTo
	(
		OwnerCharacter->GetCapsuleComponent(),
		location,
		rotation,
		false,
		false,
		0.2f,
		false,
		EMoveComponentAction::Move,
		info
	);
}

void ACDoAction_GreatSword::ExecutionFunction_SpecialAction()
{
	OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACDoAction_GreatSword::End_SpecialAction()
{
	Super::End_SpecialAction();

	State->SetIdleMode();
	Status->SetMove();
	HitCharacters.Empty();

	IsDoAction = true;
	IsAirAction = false;
	IsSkillAction = false;
	IsSpecialAction = false;

	UCBehaviorCompoent* behavior = CHelpers::GetComponent<UCBehaviorCompoent>(OwnerCharacter);
	CheckNull(behavior);

	behavior->SetEndSkillAction(true);
}

void ACDoAction_GreatSword::SummonAction()
{
	Super::SummonAction();

	IsDoAction = false;
	IsAirAction = false;
	IsSkillAction = false;
	IsSpecialAction = false;
	IsSummonAction = true;

	State->SetSummonActionMode();

	const FSummonActionData& data = SummonActionDatas[SummonIndex];
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);

	if (data.bCanMove == true)
		Status->SetMove();
	else
		Status->SetStop();

	SummonController = Cast<ACSummonController>(UGameplayStatics::GetActorOfClass(GetWorld(), data.SummonControllerClass));
	if (SummonController != NULL)
	{
		//CLog::Log("Find SummonController");
		SummonController->SetSummoner(OwnerCharacter);
		SummonController->OnSummon();
	}
}

void ACDoAction_GreatSword::BeginSummonAction()
{
	Super::BeginSummonAction();

	const FSummonActionData& data = SummonActionDatas[SummonIndex];

	FTransform transform;

	ACharacter* target = Cast<ACharacter>(Behavior->GetTargetPlayer());
	CheckNull(target);

	FVector startLocation = target->GetActorLocation() + (target->GetActorRightVector() * 500.0f);
	FVector endLocation = target->GetActorLocation() + (target->GetActorRightVector() * 500.0f * -1.0f);

	transform.SetLocation(startLocation);
	
	CannonTarget = GetWorld()->SpawnActorDeferred<ACCannonTarget>(data.CannonTargetClass, transform, OwnerCharacter, NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	CannonTarget->OnCannonBeginOverlap.AddDynamic(this, &ACDoAction_GreatSword::OnCannonBeginOverlap);
	CannonTarget->OnCannonEndOverlap.AddDynamic(this, &ACDoAction_GreatSword::OnCannonEndOverlap);
	CannonTarget->SetStartLocation(startLocation);
	CannonTarget->SetEndLocation(endLocation);

	UGameplayStatics::FinishSpawningActor(CannonTarget, transform);

	FVector location = OwnerCharacter->GetActorLocation();
	location.Z += 400.0f;
	transform.SetLocation(location);

	Cannon = GetWorld()->SpawnActorDeferred<ACCannon>(data.CannonClass, transform, OwnerCharacter, NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	if (CannonTarget != NULL)
		Cannon->SetTarget(CannonTarget);
	UGameplayStatics::FinishSpawningActor(Cannon, transform);
	
	CannonTarget->SetCannon(Cannon);
	CannonTarget->StartMove();
}

void ACDoAction_GreatSword::EndSummonAction()
{
	Super::EndSummonAction();

	State->SetIdleMode();
	Status->SetMove();
	Cannon = NULL;
	CannonTarget = NULL;
	SummonController = NULL;

	IsDoAction = true;
	IsAirAction = false;
	IsSkillAction = false;
	IsSpecialAction = false;
	IsSummonAction = false;
}

void ACDoAction_GreatSword::Reset()
{
	Super::Reset();

	State->SetIdleMode();
	Status->SetMove();

	HitCharacters.Empty();
	ComboIndex = 0;
	bDoAction = true;;
	bBeginDoAction = true;
	AirComboIndex = 0;
	bAirAction = true;
	bBeginAirAction = true;

	IsDoAction = true;
	IsAirAction = false;
	IsSkillAction = false;
	IsSpecialAction = false;
	IsSummonAction = false;
}

void ACDoAction_GreatSword::SetSkillIndex(int32 InIndex)
{
	SkillIndex = InIndex;
}

void ACDoAction_GreatSword::CreateSkillEffect()
{
	SetSpawnSkillThrowLocation();

	FTransform transform;
	FActorSpawnParameters params;
	params.Owner = OwnerCharacter;

	switch (SkillIndex)
	{
		case 0:
		{
			ACharacter* target = Cast<ACharacter>(Behavior->GetTargetPlayer());
			CheckNull(target);

			for (FVector skillEffectLocation : SkillEffectLocations)
			{
				if (SkillActionDatas[0].SkillEffectClass != NULL)
				{
					ACSkillEffect* skillEffect;
					transform.SetLocation(skillEffectLocation);

					FRotator r = UKismetMathLibrary::FindLookAtRotation(skillEffectLocation, target->GetActorLocation());
					r.Normalize();
					transform.SetRotation(FQuat(r));
					//transform.SetRotation(FQuat(OwnerCharacter->GetActorRotation()));
					skillEffect = GetWorld()->SpawnActor<ACSkillEffect>(SkillActionDatas[0].SkillEffectClass, transform, params);
					SkillEffects.Add(skillEffect);
					//UKismetSystemLibrary::K2_SetTimer(this, FString("OnDelayCreateSkillEffect"), 0.4f, false);
				}
			}
		}
			break;

		case 1:
		{
			

		}
			break;

		case 2:
		{
			HitGround();
		}
			break;

		default:
			break;
	}
}

void ACDoAction_GreatSword::OnCannonBeginOverlap(FHitResult InHitResult, ACharacter* InOtherCharacter)
{
	CheckNull(InOtherCharacter);
	CheckNull(InHitResult.GetActor());

	FString name = InOtherCharacter->GetName();
	CheckTrue(name.Contains("Enemy"));

	for (const ACharacter* hitCharacter : HitCharacters)
	{
		if (InOtherCharacter == hitCharacter)
			return;
	}
	HitCharacters.AddUnique(InOtherCharacter);

	const FSummonActionData& data = SummonActionDatas[SummonIndex];
	ReadData(data, InOtherCharacter);
	float power = data.Power;

	FDamageEvent e;
	InOtherCharacter->TakeDamage(power, e, OwnerCharacter->GetController(), this);
}

void ACDoAction_GreatSword::OnCannonEndOverlap(ACharacter* InOtherCharacter)
{
	CheckNull(InOtherCharacter);

	FString name = InOtherCharacter->GetName();
	CheckTrue(name.Contains("Enemy"));
	
	HitCharacters.Empty();
}

void ACDoAction_GreatSword::OnThrowBeginOverlap(FHitResult InHitResult, ACharacter* InOtherCharacter)
{
	CheckNull(InOtherCharacter);
	CheckNull(InHitResult.GetActor());

	for (const ACharacter* hitCharacter : HitCharacters)
	{
		if (InOtherCharacter == hitCharacter)
			return;
	}
	HitCharacters.AddUnique(InOtherCharacter);
	
	const FDoActionData& data = SkillActionDatas[SkillIndex];

	TArray<UParticleSystem*> hitEffects = data.HitEffects;
	if (hitEffects.Num() > 0)
	{
		for (UParticleSystem* hitEffect : hitEffects)
		{
			FTransform transform = data.HitEffectTransform;
			transform.AddToTranslation(InOtherCharacter->GetActorLocation());
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, transform);
		}
	}

	FString name = InOtherCharacter->GetName();
	CheckTrue(name.Contains("Enemy"));

	if (data.IsLaunch == true)
	{
		FVector deltaLocation = OwnerCharacter->GetActorForwardVector() * data.LaunchValue;
		InOtherCharacter->AddActorWorldOffset(deltaLocation);
	}

	float power = data.Power;

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

void ACDoAction_GreatSword::OnGreatSwordSkillBeginOverlap(FHitResult InHitResult, ACharacter* InOtherCharacter)
{
	CheckNull(InOtherCharacter);
	CheckNull(InHitResult.GetActor());

	FString name = InOtherCharacter->GetName();
	CheckTrue(name.Contains("Enemy"));

	for (const ACharacter* hitCharacter : HitCharacters)
	{
		if (InOtherCharacter == hitCharacter)
			return;
	}
	HitCharacters.AddUnique(InOtherCharacter);

	const FSkillActionData& data = SkillActionDatas[SkillIndex];
	ReadData(data, InOtherCharacter);
	float power = data.Power;

	FDamageEvent e;
	InOtherCharacter->TakeDamage(power, e, OwnerCharacter->GetController(), this);
}

void ACDoAction_GreatSword::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOtherCharacter);
	CheckNull(InOtherCharacter);

	FString name = InOtherCharacter->GetName();
	CheckTrue(name.Contains("Enemy"));

	for (const ACharacter* hitCharacter : HitCharacters)
	{
		if (InOtherCharacter == hitCharacter)
			return;
	}
	HitCharacters.AddUnique(InOtherCharacter);

	float power = 0.0f;

	if (IsDoAction == true)
	{
		const FDoActionData& data = DoActionDatas[ComboIndex];
		ReadData(data, InOtherCharacter);
		power = data.Power;

	} // if (IsAirAction == false)

	if (IsAirAction == true)
	{
		const FAirActionData& data = AirActionDatas[AirComboIndex];
		ReadData(data, InOtherCharacter);
		power = data.Power;
	}

	if (IsSkillAction == true)
	{
		const FSkillActionData& data = SkillActionDatas[SkillIndex];
		ReadData(data, InOtherCharacter);
		power = data.Power;
	}

	if (IsSpecialAction == true)
	{
		const FSpecialActionData& data = SpecialActionDatas[SpecialActionIndex];
		ReadData(data, InOtherCharacter);
		power = data.Power;
	}

	FDamageEvent e;
	InOtherCharacter->TakeDamage(power, e, OwnerCharacter->GetController(), this);

	if (IsDoAction == true)
		bBeginDoAction = true;

	if (IsAirAction == true)
		bBeginAirAction = true;
}

void ACDoAction_GreatSword::OnAttachmentEndOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentEndOverlap(InAttacker, InAttackCauser, InOtherCharacter);

	HitCharacters.Empty();
}

void ACDoAction_GreatSword::HitStopRestore()
{
	Super::HitStopRestore();

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);
}

void ACDoAction_GreatSword::SetSpawnDecalLoaction()
{
	ACharacter* target = Cast<ACharacter>(Behavior->GetTargetPlayer());
	CheckNull(target);

	switch (SkillIndex)
	{
		case 0:
		{
			FVector v = target->GetActorLocation();
			v.Z -= 90.0f;
			FVector v1 = v + target->GetActorForwardVector() * 250.0f;
			FVector v2 = v - (target->GetActorForwardVector() * 250.0f);
			FVector v3 = v + (target->GetActorRightVector() * 250.0f);
			FVector v4 = v - (target->GetActorRightVector() * 250.0f);
			FVector v5 = v - (target->GetActorRightVector() * 250.0f) + (target->GetActorForwardVector() * 250.0f);
			FVector v6 = v + (target->GetActorRightVector() * 250.0f) + (target->GetActorForwardVector() * 250.0f);
			FVector v7 = v - (target->GetActorRightVector() * 250.0f) - (target->GetActorForwardVector() * 250.0f);
			FVector v8 = v + (target->GetActorRightVector() * 250.0f) - (target->GetActorForwardVector() * 250.0f);

			DecalLocations.Add(v);
			DecalLocations.Add(v1);
			DecalLocations.Add(v2);
			DecalLocations.Add(v3);
			DecalLocations.Add(v4);
			DecalLocations.Add(v5);
			DecalLocations.Add(v6);
			DecalLocations.Add(v7);
			DecalLocations.Add(v8);
		}
			break;
		
		case 2:
		{
			FVector v = target->GetActorLocation();
			v.Z -= 90.0f;
			DecalLocations.Add(v);
		}
			break;

		default:
			break;
	}


}

void ACDoAction_GreatSword::SetSpawnSkillThrowLocation()
{
	FVector v = OwnerCharacter->GetActorLocation() + (OwnerCharacter->GetActorUpVector() * 300.0f);
	FVector v1 = v + (OwnerCharacter->GetActorRightVector() * 275.0f);
	FVector v2 = v - (OwnerCharacter->GetActorRightVector() * 275.0f);
	FVector v3 = v + (OwnerCharacter->GetActorUpVector() * 175.0f);
	FVector v4 = v - (OwnerCharacter->GetActorUpVector() * 175.0f);
	FVector v5 = v + (OwnerCharacter->GetActorUpVector() * 175.0f) - (OwnerCharacter->GetActorRightVector() * 275.0f);
	FVector v6 = v + (OwnerCharacter->GetActorUpVector() * 175.0f) + (OwnerCharacter->GetActorRightVector() * 275.0f);
	FVector v7 = v - (OwnerCharacter->GetActorUpVector() * 175.0f) - (OwnerCharacter->GetActorRightVector() * 275.0f);
	FVector v8 = v - (OwnerCharacter->GetActorUpVector() * 175.0f) + (OwnerCharacter->GetActorRightVector() * 275.0f);

	SkillEffectLocations.Add(v);
	SkillEffectLocations.Add(v1);
	SkillEffectLocations.Add(v2);
	SkillEffectLocations.Add(v3);
	SkillEffectLocations.Add(v4);
	SkillEffectLocations.Add(v5);
	SkillEffectLocations.Add(v6);
	SkillEffectLocations.Add(v7);
	SkillEffectLocations.Add(v8);
}

void ACDoAction_GreatSword::CreateSkillThrow()
{
	FActorSpawnParameters params;
	params.Owner = OwnerCharacter;

	switch (SkillIndex)
	{
		case 0:
		{
			int32 i = 8;

			bool bVaild = false;

			for (FVector location : SkillEffectLocations)
			{
				if (i < 0)
					return;

				bVaild = Decals.IsValidIndex(i);

				FTransform transform;
				transform.SetLocation(location);
				
				FRotator rotator = UKismetMathLibrary::FindLookAtRotation(location, Decals[i]->GetActorLocation());
				transform.SetRotation(FQuat(rotator));

				ACThrow* skillThrow = GetWorld()->SpawnActorDeferred<ACThrow>(SkillActionDatas[0].ThrowClass, transform, OwnerCharacter, NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
				skillThrow->SetIsGreatSword_Skill00(true);
				skillThrow->OnThrowBeginOverlap.AddDynamic(this, &ACDoAction_GreatSword::OnThrowBeginOverlap);
			
				if (bVaild == true)
				{
					skillThrow->GetProjectile()->HomingTargetComponent = Decals[i]->GetScene();
					skillThrow->GetProjectile()->bIsHomingProjectile = true;
				}

				UGameplayStatics::FinishSpawningActor(skillThrow, transform);
				i--;
			}
	
		}
			break;
		
		default:
			break;
	}
}

void ACDoAction_GreatSword::CreateSkillDecal()
{
	CheckTrue(DecalLocations.Num() < 0);

	ACharacter* target = Cast<ACharacter>(Behavior->GetTargetPlayer());
	CheckNull(target);

	FActorSpawnParameters params;
	params.Owner = OwnerCharacter;

	int i = 0;

	for (FVector location : DecalLocations)
	{
		FTransform transform;
		transform.SetLocation(location);
		
		if (i == 0)
			transform.SetScale3D(FVector(3.0f, 3.0f, 3.0f));

		ACCreateDecal* skillDecal = GetWorld()->SpawnActorDeferred<ACCreateDecal>(SkillActionDatas[SkillIndex].DecalClass, transform, OwnerCharacter, NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		Decals.Add(skillDecal);

		UCStateComponent* targetState = CHelpers::GetComponent<UCStateComponent>(target);
		CheckNull(targetState);

		if (targetState->IsFlying() == true)
		{
			skillDecal->GetDecal()->SetVisibility(false);
			skillDecal->SetDisappear();
		}

		if (i > 0)
			skillDecal->SetDisappear();

		UGameplayStatics::FinishSpawningActor(skillDecal, transform);

		i++;
	}
}

void ACDoAction_GreatSword::SpawnGreatSword()
{
	const FSkillActionData& data = SkillActionDatas[SkillIndex];

	FTransform transform;
	FActorSpawnParameters params;
	params.Owner = OwnerCharacter;

	FVector location = OwnerCharacter->GetActorLocation() + (OwnerCharacter->GetActorRightVector() * -1.0f * Radius);
	location.Z += 400.0f;
	transform.SetLocation(location);
	transform.SetRotation(FQuat(OwnerCharacter->GetActorRotation()));

	GreatSword = GetWorld()->SpawnActorDeferred<ACSkillEffect>(data.SkillEffectClass, transform, OwnerCharacter, NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	GreatSword->OnGreatSwordSkillBeginOverlap.AddDynamic(this, &ACDoAction_GreatSword::OnGreatSwordSkillBeginOverlap);
	UGameplayStatics::FinishSpawningActor(GreatSword, transform);
	GreatSword->OnDissolve();

	UKismetSystemLibrary::K2_SetTimer(this, "TunrGreatSword", 1.5f, false);
}

void ACDoAction_GreatSword::TunrGreatSword()
{
	FLatentActionInfo info;
	info.CallbackTarget = this;
	info.UUID = GetUniqueID();
	info.ExecutionFunction = "EndTunrGreatSword";
	info.Linkage = 0;

	FVector location = GreatSword->GetActorLocation() + (GreatSword->GetActorRightVector() * 60.0f);
	location.Z -= 400.0f;
	FRotator rotation = GreatSword->GetActorRotation();
	rotation.Yaw += 90.0f;
	rotation.Pitch += 90.0f;

	UKismetSystemLibrary::MoveComponentTo
	(
		GreatSword->GetScene(),
		location,
		rotation,
		false,
		false,
		1.0f,
		false,
		EMoveComponentAction::Move,
		info
	);
}

void ACDoAction_GreatSword::SwingGreatSword()
{
	GreatSword->SetSwing(true);

	if (GreatSword->GetNiagara() != NULL)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation
		(
			GetWorld(),
			GreatSword->GetNiagara(),
			OwnerCharacter->GetActorLocation(),
			OwnerCharacter->GetActorRotation(),
			FVector(3.0f,3.0f,3.0f)
		);
	}
}

void ACDoAction_GreatSword::EndSwing()
{
	// SwingGreatSword() - MoveComponentTo_ExecutionFunction 
	if (GreatSword != NULL)
	{
		GreatSword->Destroy();
		GreatSword = NULL;
		//CLog::Log("GreatSword->Destroy()");
	}
}

void ACDoAction_GreatSword::EndTunrGreatSword()
{
	//CLog::Log("EndTunrGreatSword");
	const FSkillActionData& data = SkillActionDatas[SkillIndex];
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, "Start");
}
// Effect 삭제 
void ACDoAction_GreatSword::DestroySkillEffects()
{
	for (ACSkillEffect* skillEffect : SkillEffects)
		skillEffect->Destroy();
}

void ACDoAction_GreatSword::HitGround()
{
	const FSkillActionData& data = SkillActionDatas[2];

	FTransform transform;
	FActorSpawnParameters params;
	params.Owner = OwnerCharacter;

	FVector location = OwnerCharacter->GetActorLocation();
	FVector direction = OwnerCharacter->GetActorForwardVector();
	location += direction * 150.0f;

	transform.AddToTranslation(location);
	FRotator rotation = OwnerCharacter->GetActorRotation();
	transform.SetRotation(FQuat(rotation));

	ACSkillEffect* skillEffect = GetWorld()->SpawnActor<ACSkillEffect>(data.SkillEffectClass, transform, params);
	skillEffect->SizeUpEffect();
	skillEffect->OnHitGround();
	SkillEffects.Add(skillEffect);
}

void ACDoAction_GreatSword::ReadData(const FDoActionData& InData, ACharacter* InOtherCharacter)
{
	if (InData.IsLaunch == true)
	{
		FVector deltaLocation = OwnerCharacter->GetActorForwardVector() * InData.LaunchValue;
		InOtherCharacter->AddActorWorldOffset(deltaLocation);
	}

	if (InData.bHitStop == true)
	{
		float hitStopTime = InData.HitStopTime;

		if (InData.bLast == true)
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
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->StartCameraShake(shake);
}

void ACDoAction_GreatSword::ReadData(const FAirActionData& InData, ACharacter* InOtherCharacter)
{
	bool StartAir = InData.StartAir;
	bool Air = InData.Air;
	bool EndAir = InData.EndAir;

	if (StartAir == true)
	{
		FVector direction = InOtherCharacter->GetActorUpVector();
		float AirLaunchValue = InData.AirLaunchValue;
		InOtherCharacter->LaunchCharacter(direction * AirLaunchValue, false, true);
		UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(InOtherCharacter);
		state->SetFlyingMode();
	}

	if (Air == true)
	{
		InOtherCharacter->GetCharacterMovement()->Velocity = FVector(0.0f, 0.0f, 0.0f);
		InOtherCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	}

	if (EndAir == true)
	{
		FVector direction = InOtherCharacter->GetActorUpVector() * -1.0f;
		float AirLaunchValue = InData.AirLaunchValue;
		InOtherCharacter->LaunchCharacter(direction * AirLaunchValue, true, true);
		UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(InOtherCharacter);
		state->SetWalkingMode();
		InOtherCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
	}

	if (InData.bHitStop == true)
	{
		float hitStopTime = InData.HitStopTime;

		if (InData.bLast == true)
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
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->StartCameraShake(shake);
}

void ACDoAction_GreatSword::ReadData(const FSkillActionData& InData, ACharacter* InOtherCharacter)
{
	if (InData.IsLaunch == true)
	{
		FVector deltaLocation = OwnerCharacter->GetActorForwardVector() * InData.LaunchValue;
		InOtherCharacter->AddActorWorldOffset(deltaLocation);
	}

	if (InData.bHitStop == true)
	{
		float hitStopTime = InData.HitStopTime;

		if (InData.bLast == true)
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
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->StartCameraShake(shake);
}

void ACDoAction_GreatSword::ReadData(const FSpecialActionData& InData, ACharacter* InOtherCharacter)
{
	if (InData.IsLaunch == true)
	{
		FVector deltaLocation = OwnerCharacter->GetActorForwardVector() * InData.LaunchValue;
		InOtherCharacter->AddActorWorldOffset(deltaLocation);
	}

	if (InData.bHitStop == true)
	{
		float hitStopTime = InData.HitStopTime;

		if (InData.bLast == true)
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


	EHittedType hittedType = InData.HittedType;
	EDeadType deadType = InData.DeadType;
	UCMontageComponent* montage = CHelpers::GetComponent<UCMontageComponent>(InOtherCharacter);
	montage->SetHittedAndDeadType(hittedType, deadType);

	TSubclassOf<UMatineeCameraShake> shake = InData.ShakeClass;
	if (shake != NULL)
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->StartCameraShake(shake);
}

void ACDoAction_GreatSword::ReadData(const FSummonActionData& InData, ACharacter* InOtherCharacter)
{
	if (InData.IsLaunch == true)
	{
		FVector deltaLocation = OwnerCharacter->GetActorForwardVector() * InData.LaunchValue;
		InOtherCharacter->AddActorWorldOffset(deltaLocation);
	}

	if (InData.bHitStop == true)
	{
		float hitStopTime = InData.HitStopTime;

		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1e-3f);
		UKismetSystemLibrary::K2_SetTimer(this, "HitStopRestore", hitStopTime * 1e-3f, false);
	} // if (InData.bHitStop == true)

	UParticleSystem* hitEffect = InData.HitEffect;
	if (hitEffect != NULL)
	{
		FTransform transform = InData.HitEffectTransform;
		transform.AddToTranslation(InOtherCharacter->GetActorLocation());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, transform);
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

	TSubclassOf<UMatineeCameraShake> shake = InData.ShakeClass;
	if (shake != NULL)
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->StartCameraShake(shake);
}
