#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CCannonTarget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCannonBeginOverlap, FHitResult, InHitResult, class ACharacter*, InOtherCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCannonEndOverlap, class ACharacter*, InOtherCharacter);


UCLASS()
class MY_01_API ACCannonTarget : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Particle")
		class UParticleSystem* HitParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Particle")
		FTransform HitParticleTransform;

	UPROPERTY(EditDefaultsOnly, Category = "Niagara")
		class UNiagaraSystem* HitNiagaraEffect;

public:
	UPROPERTY(EditAnywhere, Category = "CannonTarget")
		FVector StartLocation;

	UPROPERTY(EditAnywhere, Category = "CannonTarget")
		FVector EndLocation;

	UPROPERTY(EditAnywhere, Category = "CannonTarget")
		FVector TargetLocation;

	UPROPERTY(EditAnywhere, Category = "CannonTarget")
		bool IsHoming = false;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;

	UPROPERTY(VisibleDefaultsOnly)
		class UParticleSystemComponent* Particle;

	UPROPERTY(VisibleDefaultsOnly)
		class UBoxComponent* Box;

public:	
	ACCannonTarget();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	UPROPERTY(BlueprintAssignable)
		FCannonBeginOverlap OnCannonBeginOverlap;

	UPROPERTY(BlueprintAssignable)
		FCannonEndOverlap OnCannonEndOverlap;

public:
	void SetStartLocation(FVector InLocation);
	void SetEndLocation(FVector InLocation);
	void SetCannon(class ACCannon* InCannon);

	void StartMove();

private:
	UFUNCTION()
		void ExecutionFunction_StartMove();

private:
	class ACCannon* Cannon;
	float Speed;

};
