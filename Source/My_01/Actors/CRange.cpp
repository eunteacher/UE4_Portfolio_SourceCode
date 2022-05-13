#include "Actors/CRange.h"
#include "Global.h"

#include "Components/ShapeComponent.h"
#include "Components/SphereComponent.h"

#include "Particles/ParticleSystemComponent.h"

#include "Components/CStateComponent.h"
#include "Components/CActionComponent.h"


ACRange::ACRange()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene");
	CHelpers::CreateComponent<USphereComponent>(this, &Sphere, "Sphere", Scene);
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Particle", Scene);
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle2, "Particle2", Scene);

	Particle->SetRelativeLocation(FVector(0.0f, 0.0f, 15.0f));
	Particle2->SetRelativeLocation(FVector(0.0f, 0.0f, 15.0f));

}

void ACRange::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<USphereComponent*> spheres;
	GetComponents<USphereComponent>(spheres);
	for (USphereComponent* sphere : spheres)
	{
		//CLog::Print(shape->GetName());
		if (sphere != NULL)
		{
			sphere->OnComponentBeginOverlap.AddDynamic(this, &ACRange::OnComponentBeginOverlap);
			sphere->OnComponentEndOverlap.AddDynamic(this, &ACRange::OnComponentEndOverlap);
		}
	}
}

void ACRange::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACRange::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckNull(OtherActor);
	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(GetOwner());
	CheckNull(action);

	CheckFalse(action->IsArcherMode());

	if (OnRangeBeginOverlap.IsBound())
	{
		OnRangeBeginOverlap.Broadcast(OtherActor);
	}
}

void ACRange::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckNull(OtherActor);
	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(GetOwner());
	CheckNull(action);

	CheckFalse(action->IsArcherMode());

	if (OnRangeEndOverlap.IsBound())
	{
		OnRangeEndOverlap.Broadcast(OtherActor);
	}
}

void ACRange::SetAppear()
{
	CheckNull(Particle);
	Particle->SetVisibility(true);
}

void ACRange::SetDisappear()
{
	CheckNull(Particle);

	Particle->SetVisibility(false);
}

void ACRange::OnWarning()
{
	CheckNull(Particle2);

	Particle2->SetVisibility(true);
}

void ACRange::OffWarning()
{
	CheckNull(Particle2);

	Particle2->SetVisibility(false);
}



