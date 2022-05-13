#include "Components/CTargetComponent.h"
#include "Global.h"

#include "Components/DecalComponent.h"

#include "Components/CActionComponent.h"

#include "GameFramework/Character.h"

UCTargetComponent::UCTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCTargetComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());

}

void UCTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CheckNull(Target);

	if(Target->GetDistanceTo(OwnerCharacter) >= TraceRadius)
	{
		EndTargeting();

		return;
	}
	
	FVector start = OwnerCharacter->GetActorLocation();
	FVector target = Target->GetActorLocation();

	FRotator rotator = UKismetMathLibrary::FindLookAtRotation(start, target);

	FRotator current = OwnerCharacter->GetControlRotation();

	rotator = UKismetMathLibrary::RInterpTo(current, rotator, DeltaTime, InteropSpeed);
	

	OwnerCharacter->GetController()->SetControlRotation(rotator);
}

void UCTargetComponent::OnTarget()
{
	if (Target != NULL)
	{
		EndTargeting();
		return;
	}

	// 여기
	StartTargeting();
}

void UCTargetComponent::EndTargeting()
{
	Target = NULL;
	Targets.Empty();

	TargetDecal->SetVisibility(false);
	TargetDecal = NULL;
}

void UCTargetComponent::StartTargeting()
{
	TraceTargets();
	SetTarget();
}

void UCTargetComponent::TraceTargets()
{
	FVector start = OwnerCharacter->GetActorLocation();
	FVector end = FVector(start.X, start.Y, start.Z + 1.0f);

	TArray<AActor*> actorsToIgnore;
	actorsToIgnore.Add(OwnerCharacter);

	TArray<FHitResult> hitResults;

	bool result = UKismetSystemLibrary::SphereTraceMultiByProfile
	(
		GetWorld(),
		start,
		end,
		TraceRadius,
		"Pawn",
		false,
		actorsToIgnore,
		DrawDebugType,
		hitResults,
		true
	);

	for (const FHitResult& hitResult : hitResults)
	{
		if (hitResult.GetActor()->GetClass() == OwnerCharacter->GetClass())
		{
			continue;
		}

		ACharacter* resultTarget = Cast<ACharacter>(hitResult.GetActor());
		
		// 여기
		//if (resultTarget->GetName().Contains("Enemy") == false)
		//{
		//	continue;
		//}

		if (resultTarget != NULL && Target == NULL)
		{
			//CLog::Log("AddUnique");
			Targets.AddUnique(resultTarget); // AddUnique 중복없이 Add
		}
	}

}

void UCTargetComponent::SetTarget()
{
	float angle = -2.0f;
	ACharacter* outTarget = NULL;

	for (ACharacter* target : Targets)
	{
		FVector direction = FQuat(OwnerCharacter->GetControlRotation()).GetForwardVector();
		FVector offset = target->GetActorLocation() - OwnerCharacter->GetActorLocation();
		offset = offset.GetSafeNormal();

		float temp = FVector::DotProduct(direction, offset);

		if (temp >= angle)
		{
			angle = temp;
			outTarget = target;
		}
	}

	ChangeTarget(outTarget);
}

void UCTargetComponent::ChangeTarget(ACharacter* InTarget)
{
	if (InTarget != NULL)
	{
		if (TargetDecal != NULL)
		{
			TargetDecal = NULL;
		}

		//CLog::Log("ChangeCuror");
		TargetDecal = CHelpers::GetComponent<UDecalComponent>(InTarget);
		CheckNull(TargetDecal);
		Target = InTarget;

		TargetDecal->SetVisibility(true);

		return;
	}

	EndTargeting();
}

