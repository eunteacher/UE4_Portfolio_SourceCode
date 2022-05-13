#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMontageComponent.h"
#include "ICharacter.h"
#include "CEnemy.generated.h"

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	Dummy, Knight, GirlKnight, Max
};

UCLASS()
class MY_01_API ACEnemy : public ACharacter, public IICharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		class UNiagaraSystem* NiagaraEffect;

protected:
	UPROPERTY(EditAnywhere)
		class UDecalComponent* Decal;

	UPROPERTY(EditAnywhere)
		class UBoxComponent* TakedownBox;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Particle")
		class UParticleSystem* WaveParticle;

protected:
	// VisibleDefaultsOnly 외부에서 건들지않는 컴포넌트
	UPROPERTY(VisibleDefaultsOnly)
		class UCFeetComponent* Feet;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;

	UPROPERTY(VisibleDefaultsOnly)
		class UCActionComponent* Action;

	UPROPERTY(VisibleDefaultsOnly)
		class  UCMontageComponent* Montage;

public:
	FORCEINLINE EEnemyType GetEnemyType() { return EnemyType; }

public:
	ACEnemy();

	void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		virtual void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType) {}

	UFUNCTION()
		virtual void OnMovementTypeChanged(EMovementType InPrevType, EMovementType InNewType) {}

	UFUNCTION()
		virtual void OnTakedownBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {}

	UFUNCTION()
		virtual void OnTakedownEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {}

	virtual void Hitted() {}
	virtual void AirHitted() {}

	virtual void Walking();
	virtual void Running();
	virtual void Flying() {}

	virtual void Dead();

public:
	virtual void FindTarget() {}

public:
	virtual void Tick(float DeltaTime) override;

protected:
	void End_Dead() override;

	void Takedown_Dead() override;

protected:
	class AController* DamageInstigator;
	EEnemyType EnemyType;
};
