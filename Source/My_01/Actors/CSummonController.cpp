#include "Actors/CSummonController.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CapsuleComponent.h"

#include "Characters/CEnemy_AI_Summon_GirlKngiht.h"

#include "Particles/ParticleSystemComponent.h"


ACSummonController::ACSummonController()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACSummonController::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> outActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), SummonClass, outActors);

	if (outActors.Num() < 0)
	{
		return;
	}

	for (AActor* outActor : outActors)
	{
		if (outActor != NULL)
		{
			//CLog::Log("Find Summon GirlKnight");
			Summons.AddUnique(Cast<ACEnemy_AI_Summon_GirlKngiht>(outActor));
		}
	}
}

void ACSummonController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACSummonController::SetSummoner(ACharacter* InSummoner)
{
	CheckNull(InSummoner);
	Summoner = InSummoner;
}

void ACSummonController::OnSummon()
{
	if (Summoner == NULL)
	{
		return;
	}

	if (Summons.Num() < 1)
	{
		return;
	}

	FVector summonLocation = Summoner->GetActorLocation() + (Summoner->GetActorForwardVector() * 300.0f * -1.0f);
	FVector summonLocation1 = summonLocation + Summoner->GetActorRightVector() * 300.0f * -1.0f;
	FVector summonLocation2 = summonLocation + Summoner->GetActorRightVector() * 300.0f;

	ACEnemy_AI_Summon_GirlKngiht* summon1 = Summons.Pop();
	ACEnemy_AI_Summon_GirlKngiht* summon2 = Summons.Pop();

	FTransform transform;
	FVector summonParticleLocation;

	if (summon1 != NULL)
	{
		if (Particle != NULL)
		{
			summonParticleLocation = summonLocation1;
			summonParticleLocation.Z -= 120.0f;
			transform.SetLocation(summonParticleLocation);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, transform);
		}

		summon1->SetActorLocation(summonLocation1);
		summon1->SetActorRotation(Summoner->GetActorRotation());
		summon1->OnDissovle();
		summon1->OnAppear();
	}

	if (summon2 != NULL)
	{
		if (Particle != NULL)
		{
			summonParticleLocation = summonLocation2;
			summonParticleLocation.Z -= 120.0f;
			transform.SetLocation(summonParticleLocation);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, transform);
		}

		summon2->SetActorLocation(summonLocation2);
		summon2->SetActorRotation(Summoner->GetActorRotation());
		summon2->OnDissovle();
		summon2->OnAppear();
	}
}

