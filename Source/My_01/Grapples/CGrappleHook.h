// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGrappleHook.generated.h"

UCLASS()
class MY_01_API ACGrappleHook : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleDefaultsOnly)
		class UProjectileMovementComponent* Projectile;

	UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent* StaticMesh;

public:
	FORCEINLINE void SetTarget(class ACGrappleTarget* InTarget) { Target = InTarget; }

public:	
	ACGrappleHook();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	float DistanceToTarget();

	UFUNCTION(BlueprintImplementableEvent)
		void OnDestroy();

private:
	FVector StartLocation;
	FVector EndLocation;
	FVector Direction;

	class ACGrappleTarget* Target;

};
