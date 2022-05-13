#include "Actions/CDoAction_Unarmed.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Camera/CameraComponent.h"

#include "Components/CStateComponent.h"
#include "Components/CMontageComponent.h"

#include "Characters/CEnemy.h"

void ACDoAction_Unarmed::TakedownAction()
{
	Super::TakedownAction();

	CheckFalse(CanTakedown);

	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::None;
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
		//CLog::Print("result True");
		VictimCharacter = Cast<ACharacter>(hitResult.Actor);
		//CLog::Print(VictimCharacter->GetName());

		OwnerCharacter->GetCharacterMovement()->DisableMovement();
		CanTakedown = false;

		State->SetTakedownActionMode();
		
		FTransform transform;
		transform.SetRotation(FQuat(VictimCharacter->GetActorRotation()));
		FVector location = VictimCharacter->GetActorLocation() + (VictimCharacter->GetActorForwardVector() * (-140.0f));
		transform.SetLocation(location);
		OwnerCharacter->SetActorTransform(transform);

		Index = UKismetMathLibrary::RandomIntegerInRange(0, TakedownActionDatas.Num() - 1);
		
		const FTakedownActionData& data = TakedownActionDatas[Index];
		float attDelay = OwnerCharacter->PlayAnimMontage(data.AttAnimMontage, data.AttPlayRatio, data.AttStartSection);
		
		EEnemyType enemyType = Cast<ACEnemy>(VictimCharacter)->GetEnemyType();
		switch(enemyType)
		{
			case EEnemyType::Dummy:
				{
					bool bHas = data.VicMaps.Contains(EEnemyType::Dummy);
					if (bHas == true)
					{
						UAnimMontage* animMontage = data.VicMaps[EEnemyType::Dummy];
						VictimCharacter->StopAnimMontage();

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
					VictimCharacter->StopAnimMontage();

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
					VictimCharacter->StopAnimMontage();

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

		FDamageEvent e;
		VictimCharacter->TakeDamage(0.0f, e, OwnerCharacter->GetController(), this);
	}

}

void ACDoAction_Unarmed::TakedownFinish()
{
	//FVector location = OwnerCharacter->GetActorLocation() + (OwnerCharacter->GetActorForwardVector() * 150.0f);
	FVector location = OwnerCharacter->GetActorLocation();

	OwnerCharacter->SetActorLocation(location);

	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	State->SetIdleMode();

	VictimCharacter = NULL;
}
