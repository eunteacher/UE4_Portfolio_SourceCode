#include "Actors/CCreateCollisionBox.h"
#include "Global.h"

#include "Particles/ParticleSystemComponent.h"

#include "Components/SphereComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"

ACCreateCollisionBox::ACCreateCollisionBox()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene");
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Particle", Scene);
	CHelpers::CreateComponent<UBoxComponent>(this, &Box, "Box", Scene);
}

void ACCreateCollisionBox::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<UShapeComponent*> shapes;
	GetComponents<UShapeComponent>(shapes);
	for (UShapeComponent* shape : shapes)
	{
		//CLog::Print(shape->GetName());
		if (shape != NULL)
		{
			shape->OnComponentBeginOverlap.AddDynamic(this, &ACCreateCollisionBox::OnComponentBeginOverlap);
			shape->OnComponentEndOverlap.AddDynamic(this, &ACCreateCollisionBox::OnComponentEndOverlap);
		}
	}
}

void ACCreateCollisionBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACCreateCollisionBox::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckNull(OtherActor);
	CheckTrue(OtherActor == GetOwner());
	CheckTrue(OtherActor->GetClass() == GetClass());

	//CLog::Log("CollisionBox OnComponentBeginOverlap");
	
	//FString otherName = OtherActor->GetName();

	if (OnCollisionBoxBeginOverlap.IsBound())
	{
		//CLog::Log("OnCollisionBoxBeginOverlap Bound");
		OnCollisionBoxBeginOverlap.Broadcast(SweepResult, Cast<ACharacter>(OtherActor));
	}

}

void ACCreateCollisionBox::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckNull(OtherActor);
	CheckTrue(OtherActor == GetOwner());
	CheckTrue(OtherActor->GetClass() == GetClass());
}

void ACCreateCollisionBox::SetIsArcher_Skill(bool InIsArcher_Skill)
{
	IsArcher_Skill = InIsArcher_Skill;
}

