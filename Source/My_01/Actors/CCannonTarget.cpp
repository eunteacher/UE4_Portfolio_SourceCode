#include "Actors/CCannonTarget.h"
#include "Global.h"

#include "Particles/ParticleSystemComponent.h"

#include "Components/SphereComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"

#include "Actors/CCannon.h"

ACCannonTarget::ACCannonTarget()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene");
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Particle", Scene);
	CHelpers::CreateComponent<UBoxComponent>(this, &Box, "Box", Scene);

	Speed = 100.0f;
}

void ACCannonTarget::BeginPlay()
{
	Super::BeginPlay();

	TArray<UBoxComponent*> boxs;
	GetComponents<UBoxComponent>(boxs);
	for (UBoxComponent* box : boxs)
	{
		//CLog::Print(shape->GetName());
		if (box != NULL)
		{
			box->OnComponentBeginOverlap.AddDynamic(this, &ACCannonTarget::OnComponentBeginOverlap);
			box->OnComponentEndOverlap.AddDynamic(this, &ACCannonTarget::OnComponentEndOverlap);
		}
	}
}

void ACCannonTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACCannonTarget::StartMove()
{
	FLatentActionInfo info;
	info.CallbackTarget = this;
	info.UUID = GetUniqueID();
	info.ExecutionFunction = "ExecutionFunction_StartMove";
	info.Linkage = 0;

	FVector location = EndLocation;
	FRotator rotation = GetActorRotation();

	UKismetSystemLibrary::MoveComponentTo
	(
		Scene,
		location,
		rotation,
		false,
		false,
		2.0f,
		false,
		EMoveComponentAction::Move,
		info
	);
}

void ACCannonTarget::ExecutionFunction_StartMove()
{
	//CLog::Log("ExecutionFunction_StartMove");
	IsHoming = true;
	Destroy();
	if (Cannon != NULL)
	{
		Cannon->Destroy();
		//CLog::Log("Cannon Destroy");
	}
}

void ACCannonTarget::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckNull(OtherActor);
	CheckTrue(OtherActor == GetOwner());
	CheckTrue(OtherActor->GetClass() == GetClass());

	if (OnCannonBeginOverlap.IsBound())
	{
		//CLog::Log("OnCollisionBoxBeginOverlap Bound");
		OnCannonBeginOverlap.Broadcast(SweepResult, Cast<ACharacter>(OtherActor));
	}

}

void ACCannonTarget::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckNull(OtherActor);
	CheckTrue(OtherActor == GetOwner());
	CheckTrue(OtherActor->GetClass() == GetClass());

	if (OnCannonEndOverlap.IsBound())
	{
		//CLog::Log("OnCollisionBoxBeginOverlap Bound");
		OnCannonEndOverlap.Broadcast(Cast<ACharacter>(OtherActor));
	}
}

void ACCannonTarget::SetStartLocation(FVector InLocation)
{
	StartLocation = InLocation;
}

void ACCannonTarget::SetEndLocation(FVector InLocation)
{
	EndLocation = InLocation;
}

void ACCannonTarget::SetCannon(ACCannon* InCannon)
{
	CheckNull(InCannon);
	Cannon = InCannon;
}