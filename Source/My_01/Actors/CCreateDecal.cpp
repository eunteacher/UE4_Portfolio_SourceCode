#include "Actors/CCreateDecal.h"
#include "Global.h"

#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"

#include "Particles/ParticleSystemComponent.h"

#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"

ACCreateDecal::ACCreateDecal()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene");
	CHelpers::CreateComponent<UDecalComponent>(this, &Decal, "Decal", Scene);
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Particle", Scene);
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle2, "Particle2", Scene);
	CHelpers::CreateComponent<USphereComponent>(this, &Sphere, "Sphere", Scene);

	UMaterialInstanceConstant* m;
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&m, "MaterialInstanceConstant'/Game/Materials/M_MagicCircle2_Inst.M_MagicCircle2_Inst'");
	Decal->SetDecalMaterial(m);
	Decal->DecalSize = FVector(256.0f, 256.0f, 256.0f);
	Decal->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	Decal->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	Decal->SetVisibility(false);
	Particle2->SetVisibility(false);
}

void ACCreateDecal::BeginPlay()
{
	Super::BeginPlay();

	totalTime = 0.0f;
	delay = 2.0f;
	
}

void ACCreateDecal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckTrue(IsArcherDecal == true);

	totalTime += DeltaTime;
	//CLog::Log(totalTime);
	if (totalTime > delay)
	{
		Destroy();
	}

}

void ACCreateDecal::SetAppear()
{
	CheckNull(Particle);
	Particle->SetVisibility(true);
}

void ACCreateDecal::SetDisappear()
{
	CheckNull(Particle);

	Particle->SetVisibility(false);
}

void ACCreateDecal::OnWarning()
{
	CheckNull(Particle2);

	Particle2->SetVisibility(true);
}

void ACCreateDecal::OffWarning()
{
	CheckNull(Particle2);

	Particle2->SetVisibility(false);
}