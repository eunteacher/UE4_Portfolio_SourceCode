#pragma once

#include "CoreMinimal.h"
#include "Characters/CEnemy.h"
#include "CEnemy_Dummy.generated.h"

UCLASS()
class MY_01_API ACEnemy_Dummy : public ACEnemy
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* HealthWidget;
	
public:
	ACEnemy_Dummy();

private:
	virtual void BeginPlay() override;

public:
	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* PawnInstigator, AActor* DamageCauser) override;

private:
	void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType) override;

	void OnMovementTypeChanged(EMovementType InPrevType, EMovementType InNewType) override;

	void OnTakedownBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	void OnTakedownEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	void Hitted() override;
	void AirHitted() override;
	void Dead() override;

	void Walking() override;
	void Flying() override;

};
