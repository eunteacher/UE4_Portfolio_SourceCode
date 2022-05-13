#include "Actions/CDoAction_Spear.h"
#include "Global.h"

#include "Actions/CAction.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CMontageComponent.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

void ACDoAction_Spear::DoAction()
{
	Super::DoAction();
	CheckFalse(DoActionDatas.Num() > 0);

	//CLog::Print("Spear DoAction");

	if (bEnable == true)
	{
		//CLog::Log("bEnable == true");
		bExist = true;
		bEnable = false;

		return;
	}

	CheckFalse(State->IsIdle());
	CheckFalse(State->IsWalking());
	CheckFalse(IsDoAction == true);
	State->SetActionMode();

	const FDoActionData& data = DoActionDatas[0]; // 1번째 DoActionData를 가져옴
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);

	if (data.bCanMove == true)
	{
		Status->SetMove();
	}
	else
	{
		Status->SetStop();
	}
}

void ACDoAction_Spear::Begin_DoAction()
{
	Super::Begin_DoAction();

	CheckFalse(bExist);
	bExist = false;

	//CLog::Log("Begin_DoAction");

	CheckTrue(State->IsFlying());

	const FDoActionData& prevData = DoActionDatas[ComboIndex];
	OwnerCharacter->StopAnimMontage(prevData.AnimMontage);

	ComboIndex++;

	const FDoActionData& data = DoActionDatas[ComboIndex];
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);

	if (data.bCanMove == true)
	{
		Status->SetMove();
	}
	else
	{
		Status->SetStop();
	}

	if (data.bLast == true)
	{
		IsLast = true;
	}
}

void ACDoAction_Spear::End_DoAction()
{
	Super::End_DoAction();

	

	const FDoActionData& data = DoActionDatas[ComboIndex];
	OwnerCharacter->StopAnimMontage(data.AnimMontage);

	Reset();
}

void ACDoAction_Spear::SkillAction()
{
}

void ACDoAction_Spear::Begin_SkillAction()
{
}

void ACDoAction_Spear::End_SkillAction()
{
}

void ACDoAction_Spear::TakedownAction()
{
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::ForDuration;
	TEnumAsByte<EObjectTypeQuery> ObjectType = EObjectTypeQuery::ObjectTypeQuery3;

	FVector start = OwnerCharacter->GetActorLocation();
	FVector end = start + (OwnerCharacter->GetActorForwardVector() * 250.0f);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwnerCharacter);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(ObjectType);
	FHitResult hitResult;

	bool result = UKismetSystemLibrary::LineTraceSingleForObjects
	(
		OwnerCharacter->GetWorld(),
		start,
		end,
		ObjectTypes,
		true,
		ActorsToIgnore,
		DrawDebugType,
		hitResult,
		true
	);

	if (result == true)
	{
		CheckFalse(State->IsIdle());
		
		VictimCharacter = Cast<ACharacter>(hitResult.Actor);
		//CLog::Print(VictimCharacter->GetName());

		float dotValue = UKismetMathLibrary::Dot_VectorVector(OwnerCharacter->GetActorForwardVector(), VictimCharacter->GetActorForwardVector());
		
		if(dotValue > 0)
		{
			// 뒤
			CheckFalse(CanTakedown);
			
			OwnerCharacter->GetCharacterMovement()->DisableMovement();

			State->SetTakedownActionMode();

			//CLog::Print("CanTakedown True");
			CanTakedown = false;

			FTransform transform;
			transform.SetRotation(FQuat(VictimCharacter->GetActorRotation()));
			FVector location = VictimCharacter->GetActorLocation() + (VictimCharacter->GetActorForwardVector() * (-80.0f));
			transform.SetLocation(location);
			OwnerCharacter->SetActorTransform(transform);

			TakedownIndex = 0;

			const FTakedownActionData& data = TakedownActionDatas[TakedownIndex];

			if (data.AttPawnControl == true)
			{
				OwnerCharacter->bUseControllerRotationYaw = false;
				OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
			}

			float attDelay = OwnerCharacter->PlayAnimMontage(data.AttAnimMontage, data.AttPlayRatio, data.AttStartSection);

			EEnemyType enemyType = Cast<ACEnemy>(VictimCharacter)->GetEnemyType();
			switch (enemyType)
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

			default:
				break;
			}

			UKismetSystemLibrary::K2_SetTimer(this, "TakedownFinish", attDelay, false);

			UCMontageComponent* vicMontage = CHelpers::GetComponent<UCMontageComponent>(VictimCharacter);
			CheckNull(vicMontage);

			EHittedType hittedType = EHittedType::Max;
			EDeadType deadType = EDeadType::Takedown;
			vicMontage->SetHittedAndDeadType(hittedType, deadType);
		}
		else
		{
			CheckTrue(CanTakedown);
			// 앞
			OwnerCharacter->GetCharacterMovement()->DisableMovement();

			State->SetTakedownActionMode();

			FTransform transform;
			FVector vic_ForwardVec = VictimCharacter->GetActorForwardVector();
			vic_ForwardVec *= (-1.0f);
			transform.SetRotation(FQuat(vic_ForwardVec.Rotation()));
			FVector location = VictimCharacter->GetActorLocation() + (VictimCharacter->GetActorForwardVector() * (150.0f));
			transform.SetLocation(location);
			OwnerCharacter->SetActorTransform(transform);

			TakedownIndex = UKismetMathLibrary::RandomIntegerInRange(1, 2);

			const FTakedownActionData& data = TakedownActionDatas[TakedownIndex];

			if (data.AttPawnControl == true)
			{
				OwnerCharacter->bUseControllerRotationYaw = false;
				OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
			}

			float attDelay = OwnerCharacter->PlayAnimMontage(data.AttAnimMontage, data.AttPlayRatio, data.AttStartSection);

			EEnemyType enemyType = Cast<ACEnemy>(VictimCharacter)->GetEnemyType();
			switch (enemyType)
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

			default:
				break;
			}

			UKismetSystemLibrary::K2_SetTimer(this, "TakedownFinish", attDelay, false);

			UCMontageComponent* vicMontage = CHelpers::GetComponent<UCMontageComponent>(VictimCharacter);
			CheckNull(vicMontage);

			EHittedType hittedType = EHittedType::Max;
			EDeadType deadType = EDeadType::Takedown;
			vicMontage->SetHittedAndDeadType(hittedType, deadType);
		}
		
	}
}

void ACDoAction_Spear::SetOwnerLocation()
{
	FVector location = VictimCharacter->GetActorLocation() + (VictimCharacter->GetActorForwardVector() * 150.0f);
	OwnerCharacter->SetActorLocation(location);
}

void ACDoAction_Spear::TakedonwSpawnHitEffect()
{
	const FTakedownActionData& data = TakedownActionDatas[TakedownIndex];
	
	UNiagaraComponent* effect = UNiagaraFunctionLibrary::SpawnSystemAtLocation
	(
		GetWorld(),
		data.NiagaraEffect,
		VictimCharacter->GetActorLocation(),
		VictimCharacter->GetActorRotation()
	);
}

void ACDoAction_Spear::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOtherCharacter);
	CheckNull(InOtherCharacter);

	// 중복 데미지 처리 
	for (const ACharacter* hitCharacter : HitCharacters)
	{
		if (InOtherCharacter == hitCharacter)
		{
			return;
		} // if (InOtherCharacter == hitCharacter)
	}
	HitCharacters.Add(InOtherCharacter);

	float power = 0.0f;

	if (IsDoAction == true)
	{
		const FDoActionData& data = DoActionDatas[ComboIndex];
		ReadData(data, InOtherCharacter);
		power = data.Power;

	} // if (IsAirAction == false)

	FDamageEvent e;
	InOtherCharacter->TakeDamage(power, e, OwnerCharacter->GetController(), this);
}

void ACDoAction_Spear::OnAttachmentEndOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentEndOverlap(InAttacker, InAttackCauser, InOtherCharacter);
	HitCharacters.Empty();
}

void ACDoAction_Spear::HitStopRestore()
{
	Super::HitStopRestore();

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);
}

void ACDoAction_Spear::ReadData(const FDoActionData& InData, ACharacter* InOtherCharacter)
{
	if (InData.IsLaunch == true)
	{
		//CLog::Print("IsLaunch Call");
		FVector direction = InOtherCharacter->GetActorLocation() - OwnerCharacter->GetActorLocation();
		direction.Normalize();
		float LaunchValue = InData.LaunchValue;
		InOtherCharacter->LaunchCharacter(direction * LaunchValue, true, false);
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
	{
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->StartCameraShake(shake);

		//UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->PlayCameraShake(shake);
	}

}

void ACDoAction_Spear::ReadData(const FAirActionData& InData, ACharacter* InOtherCharacter)
{
}

void ACDoAction_Spear::ReadData(const FSkillActionData& InData, ACharacter* InOtherCharacter)
{
}

void ACDoAction_Spear::Reset()
{
	IsDoAction = true;
	IsSkillAction = false;

	State->SetIdleMode();
	Status->SetMove();

	ComboIndex = 0;
	TakedownIndex = 0;
	SkillIndex = 0;
	IsLast = false;
}

void ACDoAction_Spear::TakedownFinish()
{
	//FVector location = OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorForwardVector() * 500.0f;
	//OwnerCharacter->SetActorLocation(location);

	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	State->SetIdleMode();

	const FTakedownActionData& data = TakedownActionDatas[TakedownIndex];
	if (data.AttPawnControl == true)
	{
		OwnerCharacter->bUseControllerRotationYaw = true;
		OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	}

}
