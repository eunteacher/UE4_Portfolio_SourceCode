#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CRange.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRangeBeginOverlap, class AActor*, InOtherActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRangeEndOverlap, class AActor*, InOtherActor);

UCLASS()
class MY_01_API ACRange : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;

	UPROPERTY(VisibleDefaultsOnly)
		class USphereComponent* Sphere;

	UPROPERTY(VisibleDefaultsOnly)
		class UParticleSystemComponent* Particle;

	UPROPERTY(VisibleDefaultsOnly)
		class UParticleSystemComponent* Particle2;

public:
	FORCEINLINE class USphereComponent* GetSphere() { return Sphere; }

public:	
	ACRange();

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
		FRangeEndOverlap OnRangeEndOverlap;

	UPROPERTY(BlueprintAssignable)
		FRangeBeginOverlap OnRangeBeginOverlap;

public:
	void SetAppear();
	void SetDisappear();

	void OnWarning();
	void OffWarning();
};
