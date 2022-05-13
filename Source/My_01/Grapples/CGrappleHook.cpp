#include "Grapples/CGrappleHook.h"
#include "Global.h"

#include "GameFramework/ProjectileMovementComponent.h"

#include "Grapples/CGrappleTarget.h"

ACGrappleHook::ACGrappleHook()
{
	PrimaryActorTick.bCanEverTick = true;
	CHelpers::CreateActorComponent<UStaticMeshComponent>(this, &StaticMesh, "StaticMesh");
	RootComponent = StaticMesh;
	
	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");
	Projectile->ProjectileGravityScale = 0.0f;
}

void ACGrappleHook::BeginPlay()
{
	Super::BeginPlay();
	
	StartLocation = GetActorLocation();

	if (Target == NULL)
	{
		CLog::Print("Target NULL");
	}

	CheckNull(Target);
	EndLocation = Target->GetActorLocation();

	Direction = EndLocation - StartLocation;
	Direction.Normalize();

	Projectile->Velocity = Direction * 2000.0f;
}

void ACGrappleHook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float start = UKismetMathLibrary::Vector_DistanceSquared(GetActorLocation(), StartLocation);
	float end = UKismetMathLibrary::Vector_DistanceSquared(Target->GetActorLocation(), EndLocation);

	if (start > end)
	{
		float distance = UKismetMathLibrary::Vector_Distance(StartLocation, EndLocation);
		FVector location = StartLocation + (Direction * distance);
		SetActorLocation(location);

		Projectile->Velocity = FVector(0.0f, 0.0f, 0.0f);
	}

}

float ACGrappleHook::DistanceToTarget()
{
	return UKismetMathLibrary::Vector_Distance(EndLocation, GetActorLocation());
}
