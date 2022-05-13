#include "Actors/CCannon.h"
#include "Global.h"

#include "Actors/CCannonTarget.h"

ACCannon::ACCannon()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACCannon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACCannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACCannon::SetTarget(ACCannonTarget* InTarget)
{
	CheckNull(InTarget);
	Target = InTarget;
}

