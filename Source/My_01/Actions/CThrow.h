#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CThrow.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FThrowBeginOverlap, FHitResult, InHitResult, class ACharacter*, InOtherCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FThrowSpecialBeginOverlap, FHitResult, InHitResult, class ACharacter*, InOtherCharacter);

UCLASS()
class MY_01_API ACThrow : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Particle")
		TArray<class UParticleSystem*> Explosions;

	UPROPERTY(EditDefaultsOnly, Category = "Particle")
		FTransform ExplosionTransform;

	UPROPERTY(EditDefaultsOnly, Category = "Particle")
		class UParticleSystem* SpecialExplosion;

	UPROPERTY(EditDefaultsOnly, Category = "Niagara")
		class UNiagaraSystem* HitNiagaraEffect;
	
protected:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;

	UPROPERTY(VisibleDefaultsOnly)
		class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleDefaultsOnly)
		class UParticleSystemComponent* Particle;

	UPROPERTY(VisibleDefaultsOnly)
		class UCapsuleComponent* Capsule;

	UPROPERTY(VisibleDefaultsOnly)
		class UProjectileMovementComponent* Projectile;

	UPROPERTY(VisibleDefaultsOnly)
		class USphereComponent* Sphere;

public:
	FORCEINLINE class UParticleSystemComponent* GetParticle() { return Particle; }
	FORCEINLINE class UProjectileMovementComponent* GetProjectile() { return Projectile; }

public:
	void SetIsArcher_Takedown(bool InIsArcher_Takedown);
	void SetIsArcher_SpecialAction(bool InIsArcher_SpecialAction);
	void SetIsGreatSword_Skill00(bool InIsGreatSword_Skill00);
	void SetIsArcher(bool InIsArcher);

public:	
	ACThrow();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	void SetTargetLocation(FVector InLocation);

protected:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	UPROPERTY(BlueprintAssignable)
		FThrowBeginOverlap OnThrowBeginOverlap;
	
	UPROPERTY(BlueprintAssignable)
		FThrowSpecialBeginOverlap OnThrowSpecialBeginOverlap;

public:
	UFUNCTION(BlueprintImplementableEvent)
		void SizeUpEffect(); // Niagara Effect 설정을 위한 함수 

private:
	TArray<class ACharacter*> HitCharacters;

	bool IsArcher;

	bool IsArcher_Takedown;
	bool IsArcher_SpecialAction;

	bool IsGreatSword_Skill00;
	FVector TargetLocation;

	float TotalTime;
	float Delay;

};

