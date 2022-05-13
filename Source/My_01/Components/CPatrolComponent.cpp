#include "Components/CPatrolComponent.h"
#include "Global.h"

#include "Actors/CPatrolPath.h"

#include "Components/SplineComponent.h"

UCPatrolComponent::UCPatrolComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCPatrolComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UCPatrolComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

bool UCPatrolComponent::GetMoveTo(FVector& OutLocation, float& OutAcceptanceRadius)
{
	OutLocation = FVector::ZeroVector;
	OutAcceptanceRadius = AcceptanceRadius;
	CheckNullResult(Path, false);

	OutLocation = Path->GetSpline()->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::World);
	return true;
}

void UCPatrolComponent::UpdateNextIndex()
{
	CheckNull(Path);

	int32 splineCount = Path->GetSpline()->GetNumberOfSplinePoints();

	if (bReverse == true)
	{
		if (Index > 0)
		{
			Index--;
			return;
		}

		if (Path->GetSpline()->IsClosedLoop())
		{
			Index = splineCount - 1;
			return;
		}

		Index = 1;
		bReverse = false;
		return;
	}

	if (Index < (splineCount - 1))
	{
		Index++;
		return;
	}

	if (Path->GetSpline()->IsClosedLoop())
	{
		Index = 0;
		return;
	}

	Index = splineCount - 2;
	bReverse = true;
}

