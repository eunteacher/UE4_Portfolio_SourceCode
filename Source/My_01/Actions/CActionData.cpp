#include "Actions/CActionData.h"
#include "Global.h"

#include "CAction.h"
#include "CAttachment.h"
#include "CEquipment.h"
#include "CDoAction.h"

#include "GameFramework/Character.h"

#include "Components/SkeletalMeshComponent.h"

void UCActionData::BeginPlay(ACharacter* InOwnerCharacter, UCAction** OutAction)
{
	FTransform transform;

	ACAttachment* Attachment = NULL;
	if (AttachmentClass != NULL)
	{
		// Attachment Ŭ���� ����
		Attachment = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACAttachment>(AttachmentClass, transform, InOwnerCharacter);
		UGameplayStatics::FinishSpawningActor(Attachment, transform);
	}

	ACEquipment* Equipment = NULL;
	if (EquipmentClass != NULL)
	{
		// Equipment Ŭ���� ����
		Equipment = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACEquipment>(EquipmentClass, transform, InOwnerCharacter);
		Equipment->AttachToComponent(InOwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		Equipment->SetEquipData(EquipmentData);
		UGameplayStatics::FinishSpawningActor(Equipment, transform);

		if (Attachment != NULL)
		{
			// ��������Ʈ ���ε�
			Equipment->OnEquipmentDelegate.AddDynamic(Attachment, &ACAttachment::OnEquip);
			Equipment->OnUnEquipmentDelegate.AddDynamic(Attachment, &ACAttachment::OnUnEquip);
		}
	}
	
	ACDoAction* DoAction = NULL;
	if (DoActionClass != NULL)
	{
		// DoAction Ŭ���� ����
		DoAction = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACDoAction>(DoActionClass, transform, InOwnerCharacter);
		DoAction->AttachToComponent(InOwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		
		DoAction->SetDoActionDatas(DoActionDatas);
		DoAction->SetAirActionDatas(AirActionDatas);
		DoAction->SetSkillActionDatas(SkillActionDatas);
		DoAction->SetTakedownActionDatas(TakedownActionDatas);
		DoAction->SetSpecialActionDatas(SpecialActionDatas);
		DoAction->SetGuardAndParrayActionDatas(GuardAndParrayActionDatas);
		DoAction->SetSummonActionDatas(SummonDatas);
		
		if (Equipment != NULL)
			DoAction->SetEquipped(Equipment->GetEquipped());

		UGameplayStatics::FinishSpawningActor(DoAction, transform);

		// ��������Ʈ ���ε�
		if (Attachment != NULL)
		{
			Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &ACDoAction::OnAttachmentBeginOverlap);
			Attachment->OnAttachmentEndOverlap.AddDynamic(DoAction, &ACDoAction::OnAttachmentEndOverlap);
		}
	}

	*OutAction = NewObject<UCAction>();
	(*OutAction)->Attachment = Attachment;
	(*OutAction)->Equipment = Equipment;
	(*OutAction)->DoAction = DoAction;
}
