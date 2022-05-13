#include "CThrow.h"
#include "Global.h"

#include "GameFramework/ProjectileMovementComponent.h"

#include "Components/SphereComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"

#include "Components/CStateComponent.h"

#include "Particles/ParticleSystemComponent.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

ACThrow::ACThrow()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene");
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &SkeletalMesh, "SkeletalMesh", Scene);
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Particle", Scene);
	CHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule", SkeletalMesh);
	CHelpers::CreateComponent<USphereComponent>(this, &Sphere, "Sphere", SkeletalMesh);

	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");

	Projectile->InitialSpeed = 2000.0f;
	Projectile->MaxSpeed = 2200.0f;
	Projectile->ProjectileGravityScale = 0.0f;
}

void ACThrow::BeginPlay()
{
	Super::BeginPlay();

	TArray<UShapeComponent*> shapes;
	GetComponents<UShapeComponent>(shapes);
	for (UShapeComponent* shape : shapes)
	{
		//CLog::Print(shape->GetName());
		if (shape != NULL)
		{
			shape->OnComponentBeginOverlap.AddDynamic(this, &ACThrow::OnComponentBeginOverlap);
			shape->OnComponentEndOverlap.AddDynamic(this, &ACThrow::OnComponentEndOverlap);
		}
	}

	if (IsGreatSword_Skill00 == true)
	{
		//CLog::Print("IsGreatSword_Skill00 == true");
		if (Projectile != NULL)
		{
			Projectile->InitialSpeed = 2000.0f;
			Projectile->MaxSpeed = 5000.0f;
		}
	}

	TotalTime = 0.0f;
	Delay = 10.0f;
}

void ACThrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACThrow::SetTargetLocation(FVector InLocation)
{
	TargetLocation = InLocation;
}

void ACThrow::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckNull(OtherActor);
	CheckTrue(OtherActor == GetOwner());
	CheckTrue(OtherActor->GetClass() == GetClass());

	if (OnThrowBeginOverlap.IsBound())
	{
		OnThrowBeginOverlap.Broadcast(SweepResult, Cast<ACharacter>(OtherActor));
	}

	if (OnThrowSpecialBeginOverlap.IsBound())
	{
		//CLog::Print("OnThrowSpecialBeginOverlap Bound");
		OnThrowSpecialBeginOverlap.Broadcast(SweepResult, Cast<ACharacter>(OtherActor));
	}

	if (IsArcher == true)
	{
		if (HitNiagaraEffect != NULL)
		{
			if (OtherActor->GetName().Contains("Enemy") == false && OtherActor->GetName().Contains("Player") == false)
			{
				//CLog::Print("Spawn");

				FRotator r = UKismetMathLibrary::FindLookAtRotation(OtherActor->GetActorLocation(), GetOwner()->GetActorLocation());

				UNiagaraFunctionLibrary::SpawnSystemAtLocation
				(
					GetWorld(),
					HitNiagaraEffect,
					GetActorLocation(),
					r
				);
			}
		}
	}

	if (IsGreatSword_Skill00 == true)
	{
		if (HitNiagaraEffect != NULL)
		{
			if (OtherActor->GetName().Contains("Enemy") == false && OtherActor->GetName().Contains("Player") == false)
			{
				//CLog::Print("Spawn");
				//FRotator r = UKismetMathLibrary::FindLookAtRotation(OtherActor->GetActorLocation(), GetOwner()->GetActorLocation());

				UNiagaraFunctionLibrary::SpawnSystemAtLocation
				(
					GetWorld(),
					HitNiagaraEffect,
					GetActorLocation(),
					GetActorRotation()
				);

				//CLog::Log("Throw HitNiagaraEffect");
			}
		}
	}
	
	Destroy();
}

void ACThrow::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckNull(OtherActor);
	CheckTrue(OtherActor == GetOwner());
	CheckTrue(OtherActor->GetClass() == GetClass());
	
}

void ACThrow::SetIsArcher_Takedown(bool InIsArcher_Takedown)
{
	IsArcher_Takedown = InIsArcher_Takedown;
}

void ACThrow::SetIsArcher_SpecialAction(bool InIsArcher_SpecialAction)
{
	IsArcher_SpecialAction = InIsArcher_SpecialAction;
}

void ACThrow::SetIsGreatSword_Skill00(bool InIsGreatSword_Skill00)
{
	IsGreatSword_Skill00 = InIsGreatSword_Skill00;
}

void ACThrow::SetIsArcher(bool InIsArcher)
{
	IsArcher = InIsArcher;
}




