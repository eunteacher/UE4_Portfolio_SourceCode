#include "Components/CFeetComponent.h"
#include "Global.h"

#include "GameFramework/Character.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "Components/CStateComponent.h"

UCFeetComponent::UCFeetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCFeetComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	ScaledHalfHeight = OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	CheckNull(State);

}

void UCFeetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckFalse(State->IsIdle());
	CheckTrue(State->IsRiding_Movement());
	CheckTrue(State->IsCrouching());
	CheckTrue(State->IsFlying());

	float leftDistance;
	FRotator leftRotation;
	Trace(LeftSocket, leftDistance, leftRotation);

	float rightDistance;
	FRotator rightRotation;
	Trace(RightSocket, rightDistance, rightRotation);

	float offset = FMath::Min(leftDistance, rightDistance);
	offset = offset > 0.0f ? 0.0f : offset;

	Data.PelvisDistance.Z = UKismetMathLibrary::FInterpTo(Data.PelvisDistance.Z, offset, DeltaTime, InterpSpeed);
	//CLog::Log(Data.PelvisDistance.Z);

	Data.LeftDistance.X = UKismetMathLibrary::FInterpTo(Data.LeftDistance.X, leftDistance - offset, DeltaTime, InterpSpeed);
	Data.RightDistance.X = UKismetMathLibrary::FInterpTo(Data.RightDistance.X, -(rightDistance - offset), DeltaTime, InterpSpeed);
	//CLog::Log(Data.RightDistance.X);

	Data.LeftRotation = UKismetMathLibrary::RInterpTo(Data.LeftRotation, leftRotation, DeltaTime, InterpSpeed);
	Data.RightRotation = UKismetMathLibrary::RInterpTo(Data.RightRotation, rightRotation, DeltaTime, InterpSpeed);

}

void UCFeetComponent::SetStop()
{
	bStop = true;
}

void UCFeetComponent::SetUse()
{
	bStop = false;
}

void UCFeetComponent::Trace(FName InSocket, float& OutDistance, FRotator& OutRotation)
{
	OutDistance = 0.0f;
	OutRotation = FRotator::ZeroRotator;

	FVector location = OwnerCharacter->GetMesh()->GetSocketLocation(InSocket);
	FVector start = FVector(location.X, location.Y, OwnerCharacter->GetActorLocation().Z);

	float traceZ = start.Z - ScaledHalfHeight - TraceDistance; // 발에서 좀 더 아래에 둠
	FVector end = FVector(location.X, location.Y, traceZ);

	TArray<AActor *> ignoreActors;
	ignoreActors.Add(OwnerCharacter);

	FHitResult hit;

	UKismetSystemLibrary::LineTraceSingle
	(
		GetWorld(), 
		start,
		end,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), 
		true,
		ignoreActors,
		DrawDebugType,
		hit,
		true,
		FLinearColor::Green,
		FLinearColor::Red
	);

	if (hit.IsValidBlockingHit() == false)
	{
		OutDistance = 0.0f;
	}
	else
	{
		float length = (hit.ImpactPoint - hit.TraceEnd).Size();
		OutDistance = OffsetDistace + (length - TraceDistance);
	}

	FVector normal = hit.ImpactNormal;
	float roll = UKismetMathLibrary::DegAtan2(normal.Y, normal.Z); // roll 구하기
	float pitch = UKismetMathLibrary::DegAtan2(normal.X, normal.Z); // pitch 구하기 
	pitch *= -1.0f;  // -를 줘야 지면과 반대방향

	OutRotation = FRotator(pitch, 0.0f, roll); // 바닥으로부터의 노말 벡터를 Rotator
}

