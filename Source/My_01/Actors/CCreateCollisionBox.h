#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CCreateCollisionBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCollisionBoxBeginOverlap, FHitResult, InHitResult, class ACharacter*, InOtherCharacter);

UCLASS()
class MY_01_API ACCreateCollisionBox : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Particle")
		class UParticleSystem* HitParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Particle")
		FTransform HitParticleTransform;

	UPROPERTY(EditDefaultsOnly, Category = "Cinematics")
		bool bCinematics = false;


private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;

	UPROPERTY(VisibleDefaultsOnly)
		class UParticleSystemComponent* Particle;

	UPROPERTY(VisibleDefaultsOnly)
		class UBoxComponent* Box;

public:
	FORCEINLINE class USceneComponent* GetScene() { return Scene; }
	FORCEINLINE class UParticleSystemComponent* GetParticle() { return Particle; }

public:	
	ACCreateCollisionBox();

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
		FCollisionBoxBeginOverlap OnCollisionBoxBeginOverlap;

public:
	void SetIsArcher_Skill(bool InIsArcher_Skill);

private:
	TArray<class ACharacter*> HitCharacters;

	bool IsArcher_Skill;
	bool InDual_Special;
};
