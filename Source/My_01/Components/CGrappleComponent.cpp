#include "Components/CGrappleComponent.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CStateComponent.h"
#include "Components/CMontageComponent.h"

#include "Components/CapsuleComponent.h"

#include "Camera/CameraComponent.h"

#include "Grapples/CGrappleTarget.h"
#include "Grapples/CGrappleHook.h"
#include "Grapples/CGrappleRope.h"

#include "C:\Program Files\Epic Games\UE_4.26\Engine\Plugins\Runtime\CableComponent\Source\CableComponent\Classes\CableComponent.h"

UCGrappleComponent::UCGrappleComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	
}

void UCGrappleComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNull(OwnerCharacter);
	
	GrappleTargetStateType = EGrappleStateType::Retracted;
}

void UCGrappleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (GrappleTargetStateType)
	{
	case EGrappleStateType::Retracted:
		Retracted();
		break;
	case EGrappleStateType::NeraingTarget:
		NeraingTarget();
		break;
	}

}

void UCGrappleComponent::OnGrapple()
{
	CheckNull(CurrentGrappleTarget);
	UCMontageComponent* montage = CHelpers::GetComponent<UCMontageComponent>(OwnerCharacter);
	CheckNull(montage);

	montage->PlayGround();
}

void UCGrappleComponent::Retracted()
{
	//CLog::Log("Call Retracted");

	FVector start = OwnerCharacter->GetActorLocation();
	float radius = MaxGrappleDistance;

	TArray<AActor*> ActorsToIgnore;
	TArray<AActor*> OutActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(ObjectType);

	TSubclassOf<ACGrappleTarget> GrappleTargetClass;

	bool result = UKismetSystemLibrary::SphereOverlapActors
	(
		GetWorld(),
		start,
		radius,
		ObjectTypes,
		GrappleTargetClass,
		ActorsToIgnore,
		OutActors
	);

	SetCurrentTarget(Cast<ACGrappleTarget>(FindBestTarget(OutActors)));
}

void UCGrappleComponent::Firing()
{
	CheckNull(GrappleHook);
	//CLog::Log("Firing");
	//CLog::Log(GrappleHook->DistanceToTarget());
	if (GrappleHook->DistanceToTarget() < HookToTargetDistance)
	{
		GrappleTargetStateType = EGrappleStateType::NeraingTarget;
	}
}

void UCGrappleComponent::NeraingTarget()
{
	//CLog::Log("NeraingTarget");
	GrappleTargetStateType = EGrappleStateType::OnTarget;
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	state->SetGrappleMode();
}

void UCGrappleComponent::OnTarget()
{
	//CLog::Log("OnTarget");
	FLatentActionInfo info;
	info.CallbackTarget = this;
	info.UUID = GetUniqueID();
	info.ExecutionFunction = "EndGrapple";
	info.Linkage = 0;

	UKismetSystemLibrary::MoveComponentTo
	(
		OwnerCharacter->GetCapsuleComponent(),
		GetDestinationLocation(),
		GetDestinationRotation(),
		false,
		false,
		0.8f,
		false,
		EMoveComponentAction::Move,
		info
	);

	GrappleTargetStateType = EGrappleStateType::Retracted;
}

FVector UCGrappleComponent::GetDestinationLocation()
{
	FVector v1 = CurrentGrappleTarget->GetActorLocation();
	FVector v2 = OwnerCharacter->GetActorLocation();

	FRotator r1 = UKismetMathLibrary::FindLookAtRotation(v1, v2);
	FVector v3 = r1.Vector();
	v3.Normalize();

	FVector destination = (v3 * 200.0f) + v1;
	destination.Z += 300.0f;

	return destination;
}

FRotator UCGrappleComponent::GetDestinationRotation()
{
	FVector v1 = OwnerCharacter->GetActorLocation();
	v1.Z = 0.0f;
	FVector v2 = CurrentGrappleTarget->GetActorLocation();
	v2.Z = 0.0f;

	return UKismetMathLibrary::FindLookAtRotation(v1, v2);
}

void UCGrappleComponent::EndGrapple()
{
	//CLog::Log("StopMove");
	SetCurrentTarget(NULL);
	GrappleRope->OnDestroy();
	GrappleRope = NULL;
	GrappleHook->OnDestroy();
	GrappleHook = NULL;

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	CheckNull(state);

	state->SetIdleMode();
}

AActor* UCGrappleComponent::FindBestTarget(TArray<class AActor*> InTargets)
{
	AActor* currentTarget = NULL;
	AActor* bestTarget = NULL;
	float forwardAngle = 0.0f;
	float currentDistance = 0.0f;
	float bestDistance = 0.0f;

	for (AActor* InTarget : InTargets)
	{
		currentTarget = InTarget;

		FVector start = OwnerCharacter->GetActorLocation();
		FVector end = currentTarget->GetActorLocation();

		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(OwnerCharacter);
		FHitResult hitResult;

		bool result = UKismetSystemLibrary::LineTraceSingle
		(
			GetWorld(),
			start,
			end,
			TraceType,
			true,
			ActorsToIgnore,
			DrawDebugType,
			hitResult,
			true
		);

		if (result == true)
		{
			if (hitResult.Actor == currentTarget && currentTarget != NULL)
			{
				// Step 1

				FVector direction = currentTarget->GetActorLocation() - OwnerCharacter->GetActorLocation();
				direction.Normalize();

				// 두 방향 사이의 각도 구하는 방법
				//UCameraComponent* camera = CHelpers::GetComponent<UCameraComponent>(OwnerCharacter);
				//currentAngle = UKismetMathLibrary::Dot_VectorVector(direction, camera->GetForwardVector());
				//currentAngle = UKismetMathLibrary::DegAcos(currentAngle);
				//CLog::Print(currentAngle);

				forwardAngle = UKismetMathLibrary::Dot_VectorVector(direction, OwnerCharacter->GetActorForwardVector());

				// Step 2
				if (forwardAngle > 0.0f) // 캐릭터의 전방
				{
					currentDistance = UKismetMathLibrary::Vector_Distance(OwnerCharacter->GetActorLocation(), currentTarget->GetActorLocation());
					if (currentDistance < 200.0f) // 캐럭터와의 거리가 200이하라면
					{
						continue;
					}

					if (currentDistance < bestDistance || bestTarget == NULL) // 제일 가까운 거리 
					{
						bestTarget = currentTarget;
						bestDistance = currentDistance;
					}
				} // 	if (forwardAngle > 0.0f)
			}
		} // if (result == true)
	}

	return bestTarget;
}

void UCGrappleComponent::SetCurrentTarget(ACGrappleTarget* InGrappleTarget)
{
	CheckTrue(CurrentGrappleTarget == InGrappleTarget);

	// Step 1 : CurrentGrappleTarget 있다면 
	if (CurrentGrappleTarget != NULL)
	{
		CurrentGrappleTarget->SetActive(false);
	}

	// Step 2 
	CurrentGrappleTarget = InGrappleTarget;

	if (CurrentGrappleTarget != NULL)
	{
		CurrentGrappleTarget->SetActive(true);
	}
}

void UCGrappleComponent::AttemptGrapple()
{
	CheckNull(CurrentGrappleTarget);
	CheckFalse(GrappleTargetStateType == EGrappleStateType::Retracted);

	ACGrappleHook* grappleHook = NULL;
	ACGrappleRope* grappleRope = NULL;
	FTransform transform;

	transform.SetLocation(OwnerCharacter->GetMesh()->GetSocketLocation("GrappleHook"));
	grappleHook = OwnerCharacter->GetWorld()->SpawnActorDeferred<ACGrappleHook>(GrappleHookClass, transform, OwnerCharacter);
	if (CurrentGrappleTarget == NULL)
	{
		CLog::Print("CurrentGrappleTarget == NULL");
		return;
	}
	grappleHook->SetTarget(CurrentGrappleTarget);
	UGameplayStatics::FinishSpawningActor(grappleHook, transform);
	GrappleHook = grappleHook;
	//CLog::Log("FinishSpawningActor");

	grappleRope = OwnerCharacter->GetWorld()->SpawnActorDeferred<ACGrappleRope>(GrappleRopeClass, transform, OwnerCharacter);
	UGameplayStatics::FinishSpawningActor(grappleRope, transform);
	GrappleRope = grappleRope;

	GrappleRope->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true), "GrappleRope");
	if (GrappleHook == NULL)
	{
		CLog::Print("GrappleHook == NULL");
		return;
	}
	GrappleRope->CableComponent->SetAttachEndTo(GrappleHook, "StaticMesh");

	OwnerCharacter->SetActorRotation(FQuat(UKismetMathLibrary::MakeRotFromZX(FVector(0.0f, 0.0f, 1.0f), TargetDirection())));

	GrappleTargetStateType = EGrappleStateType::Firing;
}

FVector UCGrappleComponent::TargetDirection()
{
	if (CurrentGrappleTarget == NULL)
	{
		CLog::Log("TargetDirection : CurrentGrappleTarget == NULL");
		return FVector(0.0f, 0.0f, 0.0f);
	}

	FVector direction = CurrentGrappleTarget->GetActorLocation();
	direction.Z += 100.0f;
	direction -= OwnerCharacter->GetActorLocation();

	return direction;
}



