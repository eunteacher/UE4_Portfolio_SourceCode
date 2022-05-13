#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CCreateDecal.generated.h"

UCLASS()
class MY_01_API ACCreateDecal : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Particle")
		class UParticleSystem* DecalParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Particle")
		FTransform DecalParticleTransform;


private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;

	UPROPERTY(VisibleDefaultsOnly)
		class UDecalComponent* Decal;

	UPROPERTY(VisibleDefaultsOnly)
		class UParticleSystemComponent* Particle;

	UPROPERTY(VisibleDefaultsOnly)
		class UParticleSystemComponent* Particle2;

	UPROPERTY(VisibleDefaultsOnly)
		class USphereComponent* Sphere;

public:
	FORCEINLINE class USceneComponent* GetScene() { return Scene; }
	FORCEINLINE class UDecalComponent* GetDecal() { return Decal; }
	FORCEINLINE class USphereComponent* GetSphere() { return Sphere; }

	FORCEINLINE void SetIsArcherDecal(bool InIsArcherDecal) { IsArcherDecal = InIsArcherDecal; }

public:	
	ACCreateDecal();

public:
	void SetAppear();
	void SetDisappear();

	void OnWarning();
	void OffWarning();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	float totalTime;
	float delay;

	bool IsArcherDecal;

};
