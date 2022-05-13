#include "Actors/CPatol.h"
#include "Global.h"

#include "Particles/ParticleSystemComponent.h"

#include "Components/BoxComponent.h"

ACPatol::ACPatol()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene");
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Particle", Scene);
	CHelpers::CreateComponent<UBoxComponent>(this, &Box, "Box", Scene);
}

void ACPatol::BeginPlay()
{
	Super::BeginPlay();

	TArray<UBoxComponent*> boxs;
	GetComponents<UBoxComponent>(boxs);
	for (UBoxComponent* box : boxs)
	{
		//CLog::Print(shape->GetName());
		if (box != NULL)
		{
			box->OnComponentBeginOverlap.AddDynamic(this, &ACPatol::OnComponentBeginOverlap);
			box->OnComponentEndOverlap.AddDynamic(this, &ACPatol::OnComponentEndOverlap);
		}
	}
}

void ACPatol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPatol::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckNull(OtherActor);

	FString name = OtherActor->GetName();

	if (name.Contains("Player") == true)
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelName);
	}

}

void ACPatol::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckNull(OtherActor);

}
