#include "Actions/CDoAction_Dual.h"
#include "Global.h"

#include "Actions/CAction.h"
#include "Actions/Dual/CSpecialEffect.h"
#include "Actors/CCreateCollisionBox.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CMontageComponent.h"
#include "Components/CTargetComponent.h"
#include "Components/CActionComponent.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

#include "Camera/CameraComponent.h"

#include "Characters/CEnemy.h"

void ACDoAction_Dual::DoAction()
{
	Super::DoAction();
	CheckFalse(Action->IsDualMode());
	CheckFalse(DoActionDatas.Num() > 0);

	// 공중 콤보 공격일 경우 
	if (IsAirAction == true && AirActionDatas.Num() > 0)
	{
		const FAirActionData& AirActionData = AirActionDatas[AirComboIndex];
		bool StartAir = AirActionData.StartAir;
		if (StartAir == true && State->IsAirAction() == false)
			AirAction();
	}

	// 콤보 공격이 가능할 경우
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
		Status->SetMove();
	else
		Status->SetStop();
}

void ACDoAction_Dual::Begin_DoAction()
{
	Super::Begin_DoAction();
	CheckFalse(Action->IsDualMode());

	if (IsSkillAction == true) // 스킬 공격일 경우 
	{
		Begin_SkillAction();
		return;
	}

	CheckFalse(bExist);
	bExist = false;

	if (IsAirAction == true) // 공중 공격일 경우 
	{
		Begin_AirAction();
		return;
	}

	CheckTrue(State->IsFlying());

	const FDoActionData& prevData = DoActionDatas[ComboIndex];
	OwnerCharacter->StopAnimMontage(prevData.AnimMontage);

	ComboIndex++;
	if (ComboIndex >= DoActionDatas.Num())
		ComboIndex -= 1;

	const FDoActionData& data = DoActionDatas[ComboIndex];
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);

	if (data.bCanMove == true)
		Status->SetMove();
	else
		Status->SetStop();

	if (data.bLast == true)
		IsLast = true;
}

void ACDoAction_Dual::End_DoAction()
{
	Super::End_DoAction();

	if (IsAirAction == true)
	{
		End_AirAction();
		return;
	}

	if (IsSkillAction == true)
	{
		End_SkillAction();
		return;
	}

	const FDoActionData& data = DoActionDatas[ComboIndex];
	OwnerCharacter->StopAnimMontage(data.AnimMontage);

	Reset();
}

void ACDoAction_Dual::AirAction()
{
	Super::AirAction();
	CheckFalse(Action->IsDualMode());
	CheckFalse(AirActionDatas.Num() > 0);

	if (State->IsAction() == true)
	{
		const FAirActionData& AirActionData = AirActionDatas[AirComboIndex];
		bool StartAir = AirActionData.StartAir;
		if (StartAir == true)
		{
			const FDoActionData& DoActionData = DoActionDatas[ComboIndex];
			OwnerCharacter->StopAnimMontage(DoActionData.AnimMontage);
			ComboIndex = 0;
			bEnable = false;
		}
	}

	if (bEnable == true)
	{
		//CLog::Log("bEnable == true");
		bExist = true;
		bEnable = false;
		return;
	}

	IsAirAction = true;
	IsDoAction = false;
	IsSkillAction = false;
	IsSpecialAction = false;

	State->SetAirActionMode();
	State->SetFlyingMode();

	const FDoActionData& DoActionData = DoActionDatas[ComboIndex];
	OwnerCharacter->StopAnimMontage(DoActionData.AnimMontage);

	const FAirActionData& AirActionData = AirActionDatas[0];
	OwnerCharacter->PlayAnimMontage(AirActionData.AnimMontage, AirActionData.PlayRatio, AirActionData.StartSection);

	// 플레이어 공중에 띄우기
	if (AirActionData.StartAir == true)
		UKismetSystemLibrary::K2_SetTimer(this, "AirLaunch", 0.35f, false);

	if (AirActionData.bCanMove == true)
		Status->SetMove();
	else
		Status->SetStop();
}

void ACDoAction_Dual::Begin_AirAction()
{
	Super::Begin_AirAction();
	CheckFalse(Action->IsDualMode());

	// Begin_DoAction()에서 콤보 체크 

	const FAirActionData& prevData = AirActionDatas[AirComboIndex];
	OwnerCharacter->StopAnimMontage(prevData.AnimMontage);

	// MovementMode 값 셋팅
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

	AirComboIndex++; // 콤보 인덱스 증가 

	// 애니메이션 실행 
	const FAirActionData& data = AirActionDatas[AirComboIndex];
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);

	if (data.bCanMove == true)
		Status->SetMove();
	else
		Status->SetStop();

	bool EndAir = data.EndAir;

	// 마지막 공중 콤보 공격일 경우
	// MovementMode 값 셋팅
	if (EndAir == true)
		OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);

}

void ACDoAction_Dual::End_AirAction()
{
	Super::End_AirAction();

	const FAirActionData& data = AirActionDatas[AirComboIndex];
	OwnerCharacter->StopAnimMontage(data.AnimMontage);

	Reset();
}

void ACDoAction_Dual::SkillAction()
{
	Super::SkillAction();
	CheckFalse(Action->IsDualMode());

	CheckTrue(IsSkillAction == true);
	CheckFalse(SkillActionDatas.Num() > 0);

	//CLog::Log("Call SkillAction");

	if (State->IsWalking() == true) // 지상이라면
	{
		//CLog::Log("IsDoAction True");
		const FDoActionData& data = DoActionDatas[ComboIndex];
		OwnerCharacter->StopAnimMontage(data.AnimMontage);
		SkillIndex = 0; // 시작 스킬 인덱스 값 셋팅

		OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		FLatentActionInfo info;
		info.CallbackTarget = this;
		info.UUID = GetUniqueID();
		info.ExecutionFunction = "ExecutionFunction_SkillAction";
		info.Linkage = 0;

		FVector skillActionLocation = OwnerCharacter->GetActorLocation();
		FVector direction = OwnerCharacter->GetActorForwardVector();
		skillActionLocation += direction * 500.0f;

		FRotator skillActionRotation = OwnerCharacter->GetActorRotation();

		UKismetSystemLibrary::MoveComponentTo
		(
			OwnerCharacter->GetCapsuleComponent(),
			skillActionLocation,
			skillActionRotation,
			false,
			false,
			1.0f,
			false,
			EMoveComponentAction::Move,
			info
		);
	}

	if (State->IsFlying() == true) // 공중이라면 
	{
		const FAirActionData& data = AirActionDatas[AirComboIndex];
		OwnerCharacter->StopAnimMontage(data.AnimMontage);
		SkillIndex = 2;
		OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		State->SetFlyingMode();
	}

	IsAirAction = false;
	IsDoAction = false;
	IsSkillAction = true;
	IsSpecialAction = false;

	State->SetSkillActionMode();
	
	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;

	// 애니메이션 실행
	const FSkillActionData& data = SkillActionDatas[SkillIndex]; // 1번째 DoActionData를 가져옴
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);

	if (data.bCanMove == true)
		Status->SetMove();
	else
		Status->SetStop();
}

void ACDoAction_Dual::Begin_SkillAction()
{
	Super::Begin_SkillAction();
	CheckFalse(Action->IsDualMode());

	const FSkillActionData& prevData = SkillActionDatas[SkillIndex]; 
	OwnerCharacter->StopAnimMontage(prevData.AnimMontage);

	SkillIndex++; // 스킬 인덱스 시작

	const FSkillActionData& data = SkillActionDatas[SkillIndex]; 
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);

	if (data.bCanMove == true)
		Status->SetMove();
	else
		Status->SetStop();

	bool EndAir = data.EndAir;

	if (EndAir == true)
		OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
}

void ACDoAction_Dual::End_SkillAction()
{
	Super::End_SkillAction();
	//CLog::Log("End SkillAction");

	const FSkillActionData& data = SkillActionDatas[SkillIndex];
	OwnerCharacter->StopAnimMontage(data.AnimMontage);

	OwnerCharacter->bUseControllerRotationYaw = true;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;

	Reset();
}

void ACDoAction_Dual::SpecialAction()
{
	Super::SpecialAction();
	CheckFalse(Action->IsDualMode());

	CheckFalse(State->IsWalking());
	CheckFalse(State->IsIdle());

	State->SetSpecialActionMode();
	OwnerCharacter->GetCharacterMovement()->DisableMovement();

	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;

	bStar = UKismetMathLibrary::RandomBool(); // 라인 혹은 스타 
	if (bStar == true)
	{
		StarCount = 0; // 별 라인 횟수 
		bAirStar = UKismetMathLibrary::RandomBool(); // 공중 혹은 지상 
	}
	
	SpecialActionMaxCount = 1; // Line을 그릴 횟수 
	if (bStar == false)
		bLoop = true;

	SpecialAction_StartLocation = OwnerCharacter->GetActorLocation(); // 현재 위치

	// 중점 
	CenterPoint = SpecialAction_StartLocation + (OwnerCharacter->GetActorForwardVector() * 1000.0f);

	// 타겟과 플레이어의 방향 
	SpecialAction_StartRotation = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), CenterPoint);
	OwnerCharacter->SetActorRotation(SpecialAction_StartRotation);

	// 정면 방향
	SpecialAction_StartForwardVec = OwnerCharacter->GetActorForwardVector();

	// 애니메이션 실행 
	if (bAirStar == false)
	{
		const FSpecialActionData& data = SpecialActionDatas[0];
		OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);

	}
	else
	{
		const FSpecialActionData& data = SpecialActionDatas[4];
		OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);
	}

	StartUpVec = OwnerCharacter->GetActorUpVector();
}

void ACDoAction_Dual::Begin_SpecialAction()
{
	Super::Begin_SpecialAction();

	// 이제부터 움직이니까 꺼주기 
	OwnerCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// SpecialEffect 
	FVector specialEffectStartLocation;
	FRotator specialEffectStartRotation;
	FVector specialEffectTargetLocation;

	// 타겟과 플레이어의 거리 
	TargetDistance = UKismetMathLibrary::Vector_Distance(SpecialAction_StartLocation, CenterPoint); // 500

	if(bStar == false) // Line
	{
		FVector location = SpecialAction_StartLocation + (SpecialAction_StartForwardVec * TargetDistance * 2.0f);
		SpecialAction_EndLocation = location;
		OwnerCharacter->SetActorLocation(location);

		specialEffectStartLocation = SpecialAction_StartLocation;
		specialEffectStartRotation = SpecialAction_StartRotation;
		specialEffectTargetLocation = SpecialAction_EndLocation;

		FVector start = SpecialAction_StartLocation;
		FVector end = SpecialAction_EndLocation;
		TArray<AActor*> actorsToIgnore;
		actorsToIgnore.Add(OwnerCharacter);
		TArray<FHitResult> hitResults;
		TArray <TEnumAsByte<EObjectTypeQuery>> objectType;
		objectType.Add(EObjectTypeQuery::ObjectTypeQuery3);

		bool result = UKismetSystemLibrary::CapsuleTraceMultiForObjects
		(
			GetWorld(),
			start,
			end,
			34.0f,
			88.0f,
			objectType,
			true,
			actorsToIgnore,
			EDrawDebugTrace::None,
			hitResults,
			true
		);

		if (result == true)
		{
			//CLog::Log("result == true");
			for (FHitResult hitResult : hitResults)
				Targets.AddUnique(Cast<ACharacter>(hitResult.Actor));
		}
	}
	else
	{
		if (bAirStar == false) // Star
		{
			FRotator r = SpecialAction_StartRotation;
			r.Yaw += 18.0f; // 18도 틀기 
			OwnerCharacter->SetActorRotation(r);
			SpecialAction_StartRotation = r;

			FVector direction = r.Vector();
			direction.Normalize();
			FVector location = SpecialAction_StartLocation + (direction * TargetDistance * UKismetMathLibrary::Cos(18.0f) * 2.0f);
			OwnerCharacter->SetActorLocation(location);
			
			StarCount = 1;

			// Effect 생성 위치와 방향
			specialEffectStartLocation = SpecialAction_StartLocation;
			specialEffectStartRotation = SpecialAction_StartRotation;
			specialEffectTargetLocation = location;
		}
		else // AirStar
		{
			OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			FLatentActionInfo info;
			info.CallbackTarget = this;
			info.UUID = GetUniqueID();
			info.ExecutionFunction = "ExecutionFunction_SpecialAction";
			info.Linkage = 0;

			FVector location = CenterPoint;
			FRotator rotation = OwnerCharacter->GetActorForwardVector().Rotation();
			
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

			specialEffectStartLocation = SpecialAction_StartLocation;
			specialEffectStartRotation = SpecialAction_StartRotation;
			specialEffectTargetLocation = location;
		}
		
		FVector start = CenterPoint;
		FVector end = CenterPoint - OwnerCharacter->GetActorUpVector();
		TArray<AActor*> actorsToIgnore;
		actorsToIgnore.Add(OwnerCharacter);
		TArray<FHitResult> hitResults;
		TArray <TEnumAsByte<EObjectTypeQuery>> objectType;
		objectType.Add(EObjectTypeQuery::ObjectTypeQuery3);
		float radius = TargetDistance;

		bool result = UKismetSystemLibrary::SphereTraceMultiForObjects
		(
			GetWorld(),
			start,
			end,
			radius,
			objectType,
			true,
			actorsToIgnore,
			EDrawDebugTrace::None,
			hitResults,
			true
		);

		if (result == true)
		{
			for (FHitResult hitResult : hitResults)
				Targets.AddUnique(Cast<ACharacter>(hitResult.Actor));
		}
	}

	TargetTakeDamage(0); // 데미지 

	if (bAirStar == false)
	{
		SpawnSpecialEffect(specialEffectStartLocation, specialEffectStartRotation, specialEffectTargetLocation, 0);
		SpawnNiagaraEffect(OwnerCharacter->GetMesh()->GetComponentLocation(), OwnerCharacter->GetMesh()->GetComponentRotation(), 0);
	}
}

void ACDoAction_Dual::End_SpecialAction()
{
	Super::End_SpecialAction();

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);

	if (bLoop == true)
	{
		if (SpecialActionCount >= SpecialActionMaxCount)
		{
			bLoop = false;
		}
		else
		{
			SpecialActionCount++;
			Loop_SpecialAction();
			return;
		}
	}
	
	OwnerCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	State->SetIdleMode();
	Status->SetMove();

	if (bAirStar == true)
		State->SetWalkingMode();

	if(bStar == false)
		OwnerCharacter->SetActorRotation(SpecialAction_StartRotation);

	OwnerCharacter->bUseControllerRotationYaw = true;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;

	SpecialActionCount = 0;
	SpecialActionMaxCount = 0;
	bLoop = false;

	bStar = false;
	StarCount = 0;

	bAirStar = false;

	// Effect 삭제
	if (Effects.Num() > 0)
	{
		for (ACSpecialEffect* effect : Effects)
			effect->Destroy();

		Effects.Empty();
	}
	
	Targets.Empty();

}

void ACDoAction_Dual::GuardAction()
{
	Super::GuardAction();

	if(State->IsGuarding())
	{
		BlockAction();
		return;
	}
	else
	{
		State->SetGuardingMode();
	}
}

void ACDoAction_Dual::EndBlockAction()
{
	Super::EndBlockAction();

	Status->SetMove();
	State->SetIdleMode();
}

void ACDoAction_Dual::ParryAction()
{
	Super::ParryAction();
	CheckFalse(bParry);
	CheckTrue(State->IsParry());

	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::None;
	TEnumAsByte<EObjectTypeQuery> ObjectType = EObjectTypeQuery::ObjectTypeQuery3;

	FVector start = OwnerCharacter->GetActorLocation();
	FVector end = start + (OwnerCharacter->GetActorForwardVector() * 150.0f);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwnerCharacter);
	FHitResult hitResult;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(ObjectType);

	bool result = UKismetSystemLibrary::LineTraceSingleForObjects
	(
		GetWorld(),
		start,
		end,
		ObjectTypes,
		true,
		ActorsToIgnore,
		DrawDebugType,
		hitResult,
		true
	);

	if(result == true)
	{
		ParryTarget = Cast<ACharacter>(hitResult.Actor);
		
		if(ParryTarget != NULL)
		{
			//CLog::Print(ParryTarget->GetName());
			State->SetParryMode();
			IsGuardAndParrayAction = true;

			const FGuardAndParrayActionData& data = GuardAndParrayActionDatas[0];
			OwnerCharacter->StopAnimMontage(data.BlockAnimMontage);

			OwnerCharacter->PlayAnimMontage(data.ParrayAnimMontage, data.ParrayPlayRatio, data.ParrayStartSection);

			if (data.ParrayCanMove == true)
				Status->SetMove();
			else
				Status->SetStop();

		
		}
	}
	//CLog::Print("ParryAction");
}

void ACDoAction_Dual::BeginParryAction()
{
	Super::BeginParryAction();
	OwnerCharacter->SetActorLocation(ParryTarget->GetActorLocation() + (ParryTarget->GetActorForwardVector() * 180.0f));
}

void ACDoAction_Dual::EndParryAction()
{
	Super::EndParryAction();

	ParryTarget = NULL;
	State->SetIdleMode();
	Status->SetMove();

	IsGuardAndParrayAction = false;
}
// 방향만 전환
void ACDoAction_Dual::Turn_SpecialAction()
{
	if(bStar == false)
	{
		FVector direction;

		if (SpecialActionCount == 0)
		{
			direction = SpecialAction_StartForwardVec * -1.0f;
			SpecialAction_EndForwardVec = direction;
			OwnerCharacter->SetActorRotation(direction.Rotation());

			const FSpecialActionData& prevData = SpecialActionDatas[0];

			OwnerCharacter->StopAnimMontage(prevData.AnimMontage);
		}
		else
		{
			direction = SpecialAction_EndForwardVec;
			OwnerCharacter->SetActorRotation(direction.Rotation());

			const FSpecialActionData& prevData = SpecialActionDatas[2];
			OwnerCharacter->StopAnimMontage(prevData.AnimMontage);
		}

		const FSpecialActionData& data = SpecialActionDatas[1];
		OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);


	}
	else
	{
		StarLoop_SpecialAction();
	}
}
// 원래 위치로 돌아오기
void ACDoAction_Dual::ComeBack_SpecialAction()
{
	FVector specialEffectStartLocation = SpecialAction_EndLocation;
	FRotator specialEffectStartRotation;
	FVector specialEffectTargetLocation;
	
	FVector direction = SpecialAction_EndForwardVec;
	specialEffectStartRotation = direction.Rotation();

	SpawnNiagaraEffect(OwnerCharacter->GetMesh()->GetComponentLocation(), OwnerCharacter->GetMesh()->GetComponentRotation(), 1);

	OwnerCharacter->SetActorLocation(SpecialAction_StartLocation);
	specialEffectTargetLocation = SpecialAction_StartLocation;

	TargetTakeDamage(1);
	
	SpawnNiagaraEffect(OwnerCharacter->GetMesh()->GetComponentLocation(), OwnerCharacter->GetMesh()->GetComponentRotation(), 1);
	SpawnSpecialEffect(specialEffectStartLocation, specialEffectStartRotation, specialEffectTargetLocation, 1);
}
// 반복 
void ACDoAction_Dual::Loop_SpecialAction()
{
	const FSpecialActionData& prevData = SpecialActionDatas[1];
	OwnerCharacter->StopAnimMontage(prevData.AnimMontage);

	OwnerCharacter->SetActorRotation(SpecialAction_StartRotation);

	const FSpecialActionData& data = SpecialActionDatas[2];
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);
}

void ACDoAction_Dual::LoopRepeat_SpecialAction()
{
	FVector specialEffectStartLocation;
	FRotator specialEffectStartRotation;
	FVector specialEffectTargetLocation;

	OwnerCharacter->SetActorLocation(SpecialAction_EndLocation);

	specialEffectStartLocation = SpecialAction_StartLocation;
	specialEffectStartRotation = SpecialAction_StartRotation;
	specialEffectTargetLocation = SpecialAction_EndLocation;

	TargetTakeDamage(2);

	SpawnNiagaraEffect(OwnerCharacter->GetMesh()->GetComponentLocation(), OwnerCharacter->GetMesh()->GetComponentRotation(), 2);
	SpawnSpecialEffect(specialEffectStartLocation, specialEffectStartRotation, specialEffectTargetLocation, 2);
}

void ACDoAction_Dual::StarLoop_SpecialAction()
{
	FVector specialEffectStartLocation = OwnerCharacter->GetActorLocation(); // 이미 한번 움직인 후 위치
	FRotator specialEffectStartRotation;
	FVector specialEffectTargetLoaction;
	
	if (StarCount == 5)
	{
		EndStar_SpecialAction();
		return;
	}
	
	FRotator r = SpecialAction_StartRotation;
	r.Yaw -= 144.0f;
	specialEffectStartRotation = r;
	OwnerCharacter->SetActorRotation(r);
	SpecialAction_StartRotation = r;
	
	if (StarCount == 1)
	{
		const FSpecialActionData& prevData = SpecialActionDatas[0];
		OwnerCharacter->StopAnimMontage(prevData.AnimMontage);
	}
	else
	{
		const FSpecialActionData& prevData = SpecialActionDatas[3];
		OwnerCharacter->StopAnimMontage(prevData.AnimMontage);
	}
	
	const FSpecialActionData& data = SpecialActionDatas[3];
	
	FVector direction = r.Vector();
	direction.Normalize();
	FVector location = OwnerCharacter->GetActorLocation() + (direction * TargetDistance * UKismetMathLibrary::Cos(18.0f) * 2.0f);
	OwnerCharacter->SetActorLocation(location);
	specialEffectTargetLoaction = location;

	StarCount++;

	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);
	
	TargetTakeDamage(3);
	SpawnNiagaraEffect(OwnerCharacter->GetMesh()->GetComponentLocation(), OwnerCharacter->GetMesh()->GetComponentRotation(), 3);
	SpawnSpecialEffect(specialEffectStartLocation, specialEffectStartRotation, specialEffectTargetLoaction, 3);
}

void ACDoAction_Dual::EndStar_SpecialAction()
{
	const FSpecialActionData& data = SpecialActionDatas[3];
	
	FRotator r = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), CenterPoint);

	OwnerCharacter->SetActorRotation(r);
}

void ACDoAction_Dual::AirLaunch_SpecialAction()
{
	if (bAirLaunch == true)
	{
		FVector location = CenterPoint + (StartUpVec * TargetDistance);
		FRotator rotation = SpecialAction_StartForwardVec.Rotation();
		AirCenterPoint = location;

		OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		FLatentActionInfo info;
		info.CallbackTarget = this;
		info.UUID = GetUniqueID();
		info.ExecutionFunction = "ExecutionFunction_SpecialAction";
		info.Linkage = 0;

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

		bAirLaunch = false;

		State->SetFlyingMode();

		return;
	}
	else
	{
		const FSpecialActionData& prevData = SpecialActionDatas[4];
		OwnerCharacter->StopAnimMontage(prevData.AnimMontage);

		const FSpecialActionData& data = SpecialActionDatas[5];
		OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);

		bAirLaunch = true;

		return;
	}

}

void ACDoAction_Dual::AirStar_Loop_SpecialAction()
{
	SpecialAction_StartLocation = OwnerCharacter->GetActorLocation();
	SpecialAction_StartRotation = OwnerCharacter->GetActorRotation();
	SpecialAction_StartForwardVec = OwnerCharacter->GetActorForwardVector();

	FVector specialEffectStartLocation = SpecialAction_StartLocation;
	FRotator specialEffectStartRotation;
	FVector specialEffectTargetLocation;

	FVector direction;

	if (StarCount == 5) 
	{
		AirStar_End_SpecialAction();
		return;
	}
	
	const FSpecialActionData& data = SpecialActionDatas[6];

	if (StarCount == 0)
	{
		FVector startLocation = OwnerCharacter->GetActorLocation() + (SpecialAction_StartForwardVec * -1.0f * TargetDistance);
		OwnerCharacter->SetActorLocation(startLocation);
		specialEffectStartLocation = startLocation;

		FRotator startRotation = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), AirCenterPoint);
		OwnerCharacter->SetActorRotation(startRotation);

		const FSpecialActionData& prevData = SpecialActionDatas[5];
		OwnerCharacter->StopAnimMontage(prevData.AnimMontage);

		FRotator r = SpecialAction_StartRotation;
		r.Yaw += 18.0f;
		OwnerCharacter->SetActorRotation(r);
		SpecialAction_StartRotation = r;

		direction = r.Vector();
		direction.Normalize();

		FVector location = startLocation + (direction * TargetDistance * 2.0f);
		OwnerCharacter->SetActorLocation(location);
		specialEffectTargetLocation = location;
	}
	else
	{
		OwnerCharacter->StopAnimMontage(data.AnimMontage);

		FRotator r = SpecialAction_StartRotation;
		r.Yaw -= 144.0f;
		specialEffectStartRotation = r;
		OwnerCharacter->SetActorRotation(r);
		SpecialAction_StartRotation = r;
	
		direction = r.Vector();
		direction.Normalize();

		FVector location = SpecialAction_StartLocation + (direction * TargetDistance * 2.0f);
		OwnerCharacter->SetActorLocation(location);
		specialEffectTargetLocation = location;
	}
	
	StarCount++;

	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);

	TargetTakeDamage(6);
	SpawnNiagaraEffect(OwnerCharacter->GetMesh()->GetComponentLocation(), OwnerCharacter->GetMesh()->GetComponentRotation(), 6);
	SpawnSpecialEffect(specialEffectStartLocation, specialEffectStartRotation, specialEffectTargetLocation, 6);
}

void ACDoAction_Dual::AirStar_End_SpecialAction()
{
	const FSpecialActionData& data = SpecialActionDatas[7];

	FVector location = AirCenterPoint;
	FRotator rotation = OwnerCharacter->GetActorRotation();
	OwnerCharacter->SetActorRotation(rotation);
	OwnerCharacter->SetActorLocation(location);

	for (ACSpecialEffect* effect : Effects)
	{
		effect->Destroy();
	}

	Effects.Empty();

	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);
}

void ACDoAction_Dual::AirStar_Last_SpecialAction()
{
	OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FLatentActionInfo info;
	info.CallbackTarget = this;
	info.UUID = GetUniqueID();
	info.ExecutionFunction = "ExecutionFunction_SpecialAction";
	info.Linkage = 0;

	FVector location = CenterPoint;
	FRotator rotation = SpecialAction_StartForwardVec.Rotation();

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

void ACDoAction_Dual::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOtherCharacter);
	CheckNull(InOtherCharacter);

	// 중복 데미지 처리 
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

	if (IsGuardAndParrayAction == true)
	{
		const FGuardAndParrayActionData& data = GuardAndParrayActionDatas[0];
		ReadData(data, InOtherCharacter);
		power = data.Power;
	}

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(InOtherCharacter);
	CheckNull(state);

	if (state->IsDead() == true)
		return;

	FDamageEvent e;
	InOtherCharacter->TakeDamage(power, e, OwnerCharacter->GetController(), this);
}

void ACDoAction_Dual::OnAttachmentEndOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentEndOverlap(InAttacker, InAttackCauser, InOtherCharacter);
	HitCharacters.Empty();
}

void ACDoAction_Dual::HitStopRestore()
{
	Super::HitStopRestore();

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);
}

void ACDoAction_Dual::AirLaunch()
{
	const FAirActionData& AirActionData = AirActionDatas[0];
	float airLaunchValue = AirActionData.AirLaunchMeValue;
	FVector direction = OwnerCharacter->GetActorUpVector();
	OwnerCharacter->LaunchCharacter(direction * airLaunchValue, false, true);
}

void ACDoAction_Dual::ExecutionFunction_SkillAction()
{
	// SkillAction() - MoveComponentTo_ExecutionFunction 
	OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACDoAction_Dual::ExecutionFunction_SpecialAction()
{
	OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACDoAction_Dual::Reset()
{
	Super::Reset();

	if (IsAirAction == true || bAirStar == true)
		OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);

	HitCharacters.Empty();

	IsDoAction = true;
	IsAirAction = false;
	IsSkillAction = false;
	IsSpecialAction = false;

	State->SetIdleMode();
	Status->SetMove();

	ComboIndex = 0;
	AirComboIndex = 0;
	SkillIndex = 0;
	IsLast = false;
}

void ACDoAction_Dual::ReadData(const FDoActionData& InData, ACharacter* InOtherCharacter)
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
	CheckNull(montage);
	montage->SetHittedAndDeadType(hittedType, deadType);

	TSubclassOf<UMatineeCameraShake> shake = InData.ShakeClass;
	if (shake != NULL)
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->StartCameraShake(shake);
}

void ACDoAction_Dual::ReadData(const FAirActionData& InData, ACharacter* InOtherCharacter)
{
	bool StartAir = InData.StartAir;
	bool Air = InData.Air;
	bool EndAir = InData.EndAir;

	ACEnemy* enemy = Cast<ACEnemy>(InOtherCharacter);
	CheckNull(enemy);

	EEnemyType enemyType = enemy->GetEnemyType();
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(enemy);
	CheckNull(state);

	if (enemyType == EEnemyType::GirlKnight && (state->IsSpecialAction() == true || state->IsSkillAction() == true))
	{
		// 공중에 안뜸
	}
	else
	{
		if (StartAir == true)
		{
			UCStateComponent* otherState = CHelpers::GetComponent<UCStateComponent>(InOtherCharacter);
			CheckNull(otherState);

			FVector direction = InOtherCharacter->GetActorUpVector();
			float AirLaunchValue = InData.AirLaunchValue;
			InOtherCharacter->LaunchCharacter(direction * AirLaunchValue, false, true);
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
			state->SetWalkingMode();
			InOtherCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
		}
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
	CheckNull(montage);

	montage->SetHittedAndDeadType(hittedType, deadType);

	TSubclassOf<UMatineeCameraShake> shake = InData.ShakeClass;
	if (shake != NULL)
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->StartCameraShake(shake);
}

void ACDoAction_Dual::ReadData(const FSkillActionData& InData, ACharacter* InOtherCharacter)
{
	bool Air = InData.Air;
	bool EndAir = InData.EndAir;

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
		CheckNull(state);
		state->SetWalkingMode();
		InOtherCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
	}

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
	CheckNull(montage);
	montage->SetHittedAndDeadType(hittedType, deadType);

	TSubclassOf<UMatineeCameraShake> shake = InData.ShakeClass;
	if (shake != NULL)
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->StartCameraShake(shake);
}

void ACDoAction_Dual::ReadData(const FGuardAndParrayActionData& InData, ACharacter* InOtherCharacter)
{
	CheckNull(ParryTarget);

	EEnemyType enemyType = Cast<ACEnemy>(ParryTarget)->GetEnemyType();
	switch (enemyType)
	{
		case EEnemyType::Dummy:
		{
			bool bHas = InData.ParrayVicMaps.Contains(EEnemyType::Dummy);
			if (bHas == true)
			{
				UAnimMontage* animMontage = InData.ParrayVicMaps[EEnemyType::Dummy];
				ParryTarget->StopAnimMontage();

				ParryTarget->PlayAnimMontage(animMontage, InData.ParrayVicPlayRatio, InData.ParrayVicStartSection);
			}
		}
			break;

		case EEnemyType::Knight:
		{
			bool bHas = InData.ParrayVicMaps.Contains(EEnemyType::Knight);
			if (bHas == true)
			{
				UAnimMontage* animMontage = InData.ParrayVicMaps[EEnemyType::Knight];
				ParryTarget->StopAnimMontage();

				ParryTarget->PlayAnimMontage(animMontage, InData.ParrayVicPlayRatio, InData.ParrayVicStartSection);
			}
		}
			break;

		case EEnemyType::GirlKnight:
		{
			bool bHas = InData.ParrayVicMaps.Contains(EEnemyType::GirlKnight);
			if (bHas == true)
			{
				UAnimMontage* animMontage = InData.ParrayVicMaps[EEnemyType::GirlKnight];
				ParryTarget->StopAnimMontage();

				ParryTarget->PlayAnimMontage(animMontage, InData.ParrayVicPlayRatio, InData.ParrayVicStartSection);
			}
		}

		default:
			break;
	}

	if (InData.bHitStop == true)
	{
		float hitStopTime = InData.HitStopTime;
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1e-3f);
		UKismetSystemLibrary::K2_SetTimer(this, "HitStopRestore", hitStopTime * 1e-3f, false);

	} // if (InData.bHitStop == true)

	TArray<UParticleSystem*> parrayEffects = InData.ParrayEffects;
	for (UParticleSystem* parrayEffect : parrayEffects)
	{
		if (parrayEffect != NULL)
		{
			FTransform transform = InData.ParrayEffectTransform;
			transform.AddToTranslation(InOtherCharacter->GetActorLocation());
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), parrayEffect, transform);
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
	CheckNull(montage);
	montage->SetHittedAndDeadType(hittedType, deadType);

	TSubclassOf<UMatineeCameraShake> shake = InData.ShakeClass;
	if (shake != NULL)
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->StartCameraShake(shake);
}

void ACDoAction_Dual::SpawnSpecialEffect(FVector InStartLocation, FRotator InStartRotation, FVector InTargetLocation, int32 InIndex)
{
	FTransform transform;
	InStartLocation.Z -= 30.0f;
	transform.AddToTranslation(InStartLocation);
	transform.SetRotation(FQuat(InStartRotation));

	const FSpecialActionData& data = SpecialActionDatas[InIndex];
	
	ACSpecialEffect* effect = OwnerCharacter->GetWorld()->SpawnActorDeferred<ACSpecialEffect>(data.EffectClass, transform, OwnerCharacter);
	effect->SetStart(InStartLocation);
	effect->SetTarget(InTargetLocation);
	UGameplayStatics::FinishSpawningActor(effect, transform);
	Effects.Add(effect);
}

void ACDoAction_Dual::SpawnNiagaraEffect(FVector InStartLocation, FRotator InStartRotation, int32 InIndex)
{
	// Spawn Niagara Effect

	const FSpecialActionData& data = SpecialActionDatas[InIndex];

	UNiagaraComponent* effect = UNiagaraFunctionLibrary::SpawnSystemAttached
	(
		data.NiagaraEffect,
		OwnerCharacter->GetMesh(),
		"EffectSocket",
		InStartLocation,
		InStartRotation,
		EAttachLocation::KeepRelativeOffset,
		true
	);
}

void ACDoAction_Dual::TargetTakeDamage(int32 InIndex)
{
	const FSpecialActionData& data = SpecialActionDatas[InIndex];

	bool StartAir = data.StartAir;
	bool Air = data.Air;
	bool EndAir = data.EndAir;

	for (ACharacter* target : Targets)
	{
		if (target == NULL)
			continue;

		bool bOverlap = false;
		for (ACharacter* hitCharacter : HitCharacters)
		{
			if (hitCharacter == target)
			{
				bOverlap = true;
				continue;
			}
		}

		if (bOverlap == true)
			continue;

		HitCharacters.AddUnique(target);

		UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(target);
		CheckNull(state);
		if (state == NULL)
			continue;

		if (state->IsDead() == true)
			continue;

		if (bAirStar == true)
		{
			ACEnemy* enemy = Cast<ACEnemy>(target);
			if (enemy == NULL)
				continue;

			EEnemyType enemyType = enemy->GetEnemyType();

			if (enemyType == EEnemyType::GirlKnight && (state->IsSpecialAction() == true || state->IsSkillAction() == true))
			{
				// 공중에 안뜸
			}
			else
			{
				if (StartAir == true)
				{
					FVector direction = target->GetActorUpVector();
					FVector location = target->GetActorLocation() + (direction * TargetDistance);
					
					target->SetActorLocation(location);

					state->SetFlyingMode();
				}

				if (Air == true)
				{
					target->GetCharacterMovement()->Velocity = FVector(0.0f, 0.0f, 0.0f);
					target->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
				}

				if (EndAir == true)
				{
					FVector direction = target->GetActorUpVector() * -1.0f;
					float AirLaunchValue = data.AirLaunchValue;
					target->LaunchCharacter(direction * AirLaunchValue, false, true);
					target->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);

					state->SetWalkingMode();
				}
			}
		}

		float power = data.Power;

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

		TArray<UParticleSystem*> hitEffects = data.HitEffects;
		for (UParticleSystem* hitEffect : hitEffects)
		{
			if (hitEffect != NULL)
			{
				FTransform transform = data.HitEffectTransform;
				transform.AddToTranslation(target->GetActorLocation());
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, transform);
			}
		}

		if (data.HitNiagaraEffect != NULL)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation
			(
				GetWorld(),
				data.HitNiagaraEffect,
				target->GetActorLocation(),
				target->GetActorRotation()
			);
		}

		EHittedType hittedType = data.HittedType;
		EDeadType deadType = data.DeadType;
		UCMontageComponent* montage = CHelpers::GetComponent<UCMontageComponent>(target);
		CheckNull(montage);
		montage->SetHittedAndDeadType(hittedType, deadType);

		TSubclassOf<UMatineeCameraShake> shake = data.ShakeClass;
		if (shake != NULL)
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->StartCameraShake(shake);

		FDamageEvent e;
		target->TakeDamage(power, e, OwnerCharacter->GetController(), this);
	}

	HitCharacters.Empty();
}

void ACDoAction_Dual::BlockAction()
{
	//CLog::Print("BlockAction");
	const FGuardAndParrayActionData& data = GuardAndParrayActionDatas[0];
	State->SetBlockMode();
	OwnerCharacter->PlayAnimMontage(data.BlockAnimMontage, data.BlockPlayRatio, data.BlockStartSection);
	
	if(data.BlockCanMove == true)
		Status->SetMove();
	else
		Status->SetStop();
}
