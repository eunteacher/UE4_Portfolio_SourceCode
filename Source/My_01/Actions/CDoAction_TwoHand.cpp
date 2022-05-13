#include "Actions/CDoAction_TwoHand.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CMontageComponent.h"

void ACDoAction_TwoHand::DoAction()
{
	Super::DoAction();
	CheckFalse(DoActionDatas.Num() > 0);
	CheckFalse(State->IsIdle());

	State->SetActionMode();
	
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

}

void ACDoAction_TwoHand::Begin_DoAction()
{
	Super::Begin_DoAction();

	const FDoActionData& prevData = DoActionDatas[ComboIndex];
	OwnerCharacter->StopAnimMontage(prevData.AnimMontage);

	ComboIndex++;

	const FDoActionData& data = DoActionDatas[ComboIndex];
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);
}

void ACDoAction_TwoHand::End_DoAction()
{
	Super::End_DoAction();

	const FDoActionData& data = DoActionDatas[ComboIndex];
	OwnerCharacter->StopAnimMontage(data.AnimMontage);

	ComboIndex = 0;
	State->SetIdleMode();
	Status->SetMove();
	HitCharacters.Empty();
}

void ACDoAction_TwoHand::Reset()
{
	Super::Reset();

	const FDoActionData& data = DoActionDatas[ComboIndex];
	OwnerCharacter->StopAnimMontage(data.AnimMontage);

	ComboIndex = 0;
	State->SetIdleMode();
	Status->SetMove();
	HitCharacters.Empty();
}

void ACDoAction_TwoHand::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOtherCharacter);
	CheckNull(InOtherCharacter);

	FString name = InOtherCharacter->GetName();
	CheckTrue(name.Contains("Enemy"));

	for (const ACharacter* hitCharacter : HitCharacters)
	{
		if (InOtherCharacter == hitCharacter)
		{
			return;
		} // if (InOtherCharacter == hitCharacter)
	}
	HitCharacters.Add(InOtherCharacter);
	//HitCharacters.Empty();

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
	{
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->StartCameraShake(shake);

		//UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->PlayCameraShake(shake);
	}

	FDamageEvent e;
	InOtherCharacter->TakeDamage(power, e, OwnerCharacter->GetController(), this);
}

void ACDoAction_TwoHand::OnAttachmentEndOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentEndOverlap(InAttacker, InAttackCauser, InOtherCharacter);

	HitCharacters.Empty();
}

void ACDoAction_TwoHand::HitStopRestore()
{
	Super::HitStopRestore();

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);
}

