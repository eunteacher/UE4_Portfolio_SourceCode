#include "Actions/CEquipment.h"
#include "Global.h"

#include "Characters/ICharacter.h"
#include "Characters/CPlayer.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CActionComponent.h"

#include "CActionData.h"
#include "CAttachment.h"
#include "CDoAction.h"


ACEquipment::ACEquipment()
{

}

void ACEquipment::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNull(OwnerCharacter);

	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	CheckNull(State);
	Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);
	CheckNull(Status);
	Action = CHelpers::GetComponent<UCActionComponent>(OwnerCharacter);
	CheckNull(Action);

}

void ACEquipment::Equip()
{
	CheckNull(Status);
	CheckNull(State);
	CheckNull(Action);
	CheckNull(OwnerCharacter);

	State->SetEquipMode();

	if (EquipData.AnimMontage != NULL)
	{
		if (Cast<ACPlayer>(OwnerCharacter) != NULL && Cast<ACPlayer>(OwnerCharacter)->GetAirSwap() == true)
		{
			OwnerCharacter->PlayAnimMontage(EquipData.AnimMontage, EquipData.PlayRatio * 5.0f, EquipData.StartSection);
		}
		else
		{
			OwnerCharacter->PlayAnimMontage(EquipData.AnimMontage, EquipData.PlayRatio, EquipData.StartSection);
		}
		
		if (EquipData.bCanMove == true)
		{
			Status->SetMove();
		}
		else
		{
			Status->SetStop();
		}
	}
	else
	{
		Begin_Equip();
		End_Equip();
	}

	if (EquipData.bPawnControl == true)
	{
		OwnerCharacter->bUseControllerRotationYaw = true;
		OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	}
}

void ACEquipment::Begin_Equip()
{
	if (OnEquipmentDelegate.IsBound())
	{
		OnEquipmentDelegate.Broadcast();
	}
}

void ACEquipment::End_Equip()
{
	CheckNull(Status);
	CheckNull(State);

	bEquipped = true;
	Status->SetMove();
	State->SetIdleMode();
}

void ACEquipment::UnEquip()
{
	CheckNull(OwnerCharacter);

	bEquipped = false;

	if (OnUnEquipmentDelegate.IsBound())
	{
		OnUnEquipmentDelegate.Broadcast();
	}

	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
}
