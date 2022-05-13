#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CFake.generated.h"

UCLASS()
class MY_01_API ACFake : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;

	UPROPERTY(VisibleDefaultsOnly)
		class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleDefaultsOnly)
		class USkeletalMeshComponent* SpecialSkeletalMesh;

	UPROPERTY(VisibleDefaultsOnly)
		class UParticleSystemComponent* Particle;
	
public:	
	ACFake();

public:
	void SetAppear();
	void SetDisappear();

	void SetSpecialAppear();
	void SetSpecialDisAppear();

public:
	UFUNCTION(BlueprintImplementableEvent)
		void OnAppear();

	UFUNCTION(BlueprintImplementableEvent)
		void OnDisappear();

public:
	UFUNCTION(BlueprintImplementableEvent)
		void SizeUpEffect();

public:
	bool GetIsAppear() { return IsAppear; }

protected:
	virtual void BeginPlay() override;

private:
	bool IsAppear;

};
