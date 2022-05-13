#include "Actions/CFake.h"
#include "Global.h"

#include "Components/SkeletalMeshComponent.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

#include "Particles/ParticleSystemComponent.h"

ACFake::ACFake()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene");
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &SkeletalMesh, "SkeletalMesh", Scene);
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &SpecialSkeletalMesh, "SpecialSkeletalMesh", Scene);
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Particle", Scene);

	Particle->SetVisibility(false);
	Particle->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));
	Particle->SetRelativeRotation(FQuat(FRotator(90.0f, 0.0f, 0.0f)));
	Particle->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.25f));

	IsAppear = false;
}

void ACFake::SetAppear()
{
	CheckNull(SkeletalMesh);
	CheckNull(SpecialSkeletalMesh);

	SkeletalMesh->SetVisibility(true);
	SpecialSkeletalMesh->SetVisibility(false);
	IsAppear = true;
}

void ACFake::SetDisappear()
{
	CheckNull(SkeletalMesh);
	CheckNull(SpecialSkeletalMesh);

	SkeletalMesh->SetVisibility(false);
	IsAppear = false;
}

void ACFake::SetSpecialAppear()
{
	CheckNull(SkeletalMesh);
	CheckNull(SpecialSkeletalMesh);
	CheckNull(Particle);

	SkeletalMesh->SetVisibility(false);
	SpecialSkeletalMesh->SetVisibility(true);
	Particle->SetVisibility(true);
	IsAppear = false;
}

void ACFake::SetSpecialDisAppear()
{
	CheckNull(Particle);
	CheckNull(SpecialSkeletalMesh);

	SpecialSkeletalMesh->SetVisibility(false);
	Particle->SetVisibility(false);
	IsAppear = false;
}

void ACFake::BeginPlay()
{
	Super::BeginPlay();
	CheckNull(SkeletalMesh);
	CheckNull(SpecialSkeletalMesh);

	SkeletalMesh->SetVisibility(false);
	SpecialSkeletalMesh->SetVisibility(false);
}


