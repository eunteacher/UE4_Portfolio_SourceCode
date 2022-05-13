#include "Actions/CAttachment.h"
#include "Global.h"

#include "GameFramework/Character.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/ShapeComponent.h"

#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"

ACAttachment::ACAttachment()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene");
}

void ACAttachment::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNull(OwnerCharacter);

	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	CheckNull(State);
	Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);
	CheckNull(Status);

	// ��� �浹ü�� �˻��ؼ� ���ε�
	GetComponents<UShapeComponent>(ShapeComponents);
	for (UShapeComponent* shapeComponent : ShapeComponents)
	{
		// �浹ü 
		if (shapeComponent != NULL)
		{
			shapeComponent->OnComponentBeginOverlap.AddDynamic(this, &ACAttachment::OnComponentBeginOverlap);
			shapeComponent->OnComponentEndOverlap.AddDynamic(this, &ACAttachment::OnComponentEndOverlap);
		}
	}

	OffCollision();

	Super::BeginPlay();
}

// ��� ����Ʈ���� ���� ������ ���� �Լ�
// ���� �̸��� �Ű������� ����
void ACAttachment::AttachTo(FName InSocketName)
{
	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), InSocketName);
}

void ACAttachment::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckNull(OtherActor);
	CheckTrue(OwnerCharacter == OtherActor); // �ڱ� �ڽ� �浹 ����
	CheckTrue(OtherActor->GetClass() == OwnerCharacter->GetClass()); // ������ �浹 ����
	CheckTrue(OtherActor->GetClass() == GetClass()); // ���Ⳣ�� �浹 ����

	// ���ε� 
	if (OnAttachmentBeginOverlap.IsBound())
		OnAttachmentBeginOverlap.Broadcast(OwnerCharacter, this, Cast<ACharacter>(OtherActor));
}

void ACAttachment::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckNull(OtherActor);

	if (OnAttachmentEndOverlap.IsBound())
		OnAttachmentEndOverlap.Broadcast(OwnerCharacter, this, Cast<ACharacter>(OtherActor));
}

void ACAttachment::OnCollision()
{
	for (UShapeComponent* shapeComponent : ShapeComponents)
	{
		if (shapeComponent != NULL)
			shapeComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void ACAttachment::OffCollision()
{
	for (UShapeComponent* shapeComponent : ShapeComponents)
	{
		if (shapeComponent != NULL)
			shapeComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

