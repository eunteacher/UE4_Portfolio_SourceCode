#include "Actions/CSkillEffect.h"
#include "Global.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/CapsuleComponent.h"

#include "Particles/ParticleSystemComponent.h"

ACSkillEffect::ACSkillEffect()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene");
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &SkeletalMesh, "SkeletalMesh", Scene);
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Particle", Scene);
	CHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule", Scene);

	Radius = 480.0f;
	Speed = 750.0f;
	Angle = 0.0f;
	bSwing = false;

	Total = 0.0f;
}

void ACSkillEffect::BeginPlay()
{
	Super::BeginPlay();

	TArray<UShapeComponent*> shapes;
	GetComponents<UShapeComponent>(shapes);
	for (UShapeComponent* shape : shapes)
	{
		if (shape != NULL)
		{
			shape->OnComponentBeginOverlap.AddDynamic(this, &ACSkillEffect::OnComponentBeginOverlap);
			shape->OnComponentEndOverlap.AddDynamic(this, &ACSkillEffect::OnComponentEndOverlap);
		}
	}
}

void ACSkillEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bSwing == false)
	{
		if (GetOwner() != NULL)
		{
			FRotator r = GetOwner()->GetActorRotation();
			Angle = r.Yaw - 180.0f;
		}
	}

	if (bSwing == true)
	{
		Total += DeltaTime * Speed;
		Angle += DeltaTime * Speed;
		//CLog::Log(Angle);
		float radAngle = UKismetMathLibrary::DegreesToRadians(Angle);
		float posX = UKismetMathLibrary::Sin(radAngle) * Radius;
		float posY = UKismetMathLibrary::Cos(radAngle) * Radius;
		
		FVector location = GetOwner()->GetActorLocation();
		location.X -= posX;
		location.Y += posY;
		SetActorLocation(location);

		FRotator rotation = GetOwner()->GetActorRotation();
		rotation.Yaw = Angle - 90.0f;
		rotation.Pitch = 90.0f;
		SetActorRotation(rotation);
		
		if (UKismetMathLibrary::Abs(Total) >= 270.0f)
		{
			bSwing = false;
			Destroy();
		}
	}
}

void ACSkillEffect::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckNull(OtherActor);
	CheckTrue(OtherActor == GetOwner());
	CheckTrue(OtherActor->GetClass() == GetClass());

	if (OnGreatSwordSkillBeginOverlap.IsBound())
	{
		OnGreatSwordSkillBeginOverlap.Broadcast(SweepResult, Cast<ACharacter>(OtherActor));
	}

	//if (OtherActor->GetName().Contains("Enemy") == false && OtherActor->GetName().Contains("Player") == false)
}

void ACSkillEffect::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckNull(OtherActor);
	CheckTrue(OtherActor == GetOwner());
	CheckTrue(OtherActor->GetClass() == GetClass());

}
