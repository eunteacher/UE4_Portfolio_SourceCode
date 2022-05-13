#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSkillEffect.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGreatSwordSkillBeginOverlap, FHitResult, InHitResult, class ACharacter*, InOtherCharacter);

UCLASS()
class MY_01_API ACSkillEffect : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Niagara")
		class UNiagaraSystem* NiagaraEffect;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;

	UPROPERTY(VisibleDefaultsOnly)
		class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleDefaultsOnly)
		class UParticleSystemComponent* Particle;

	UPROPERTY(VisibleDefaultsOnly)
		class UCapsuleComponent* Capsule;

public:
	FORCEINLINE class USkeletalMeshComponent* GetSkeletalMesh() { return SkeletalMesh; }
	FORCEINLINE class USceneComponent* GetScene() { return Scene; }
	FORCEINLINE class UNiagaraSystem* GetNiagara() { return NiagaraEffect; }
	FORCEINLINE void SetSwing(bool InSwing) { bSwing = InSwing; }

public:	
	ACSkillEffect();

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
		FGreatSwordSkillBeginOverlap OnGreatSwordSkillBeginOverlap;

public:
	UFUNCTION(BlueprintImplementableEvent)
		void SizeUpEffect();

	UFUNCTION(BlueprintImplementableEvent)
		void OnDissolve();

	UFUNCTION(BlueprintImplementableEvent)
		void OnTurn();

	UFUNCTION(BlueprintImplementableEvent)
		void OnNiagara_Slash();

	UFUNCTION(BlueprintImplementableEvent)
		void OnNiagara_GreatSword();

	UFUNCTION(BlueprintImplementableEvent)
		void OnNiagara_DestroySlash();

	UFUNCTION(BlueprintImplementableEvent)
		void OnHitGround();


private:
	float Radius;
	float Speed;
	float Angle;
	bool bSwing;

	float Total;
	
};
