#pragma once

#include "CoreMinimal.h"
#include "Characters/CEnemy_AI.h"
#include "CEnemy_AI_Summon_GirlKngiht.generated.h"

UCLASS()
class MY_01_API ACEnemy_AI_Summon_GirlKngiht : public ACEnemy_AI
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* HealthWidget;


public:
	ACEnemy_AI_Summon_GirlKngiht();

private:
	void BeginPlay() override;

public:
	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* PawnInstigator, AActor* DamageCauser) override;

private:
	void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType) override;
	void OnMovementTypeChanged(EMovementType InPrevType, EMovementType InNewType) override;

	void Hitted() override;
	void AirHitted() override;
	void Dead() override;

	void Walking() override;
	void Flying() override;

public:
	UFUNCTION(BlueprintImplementableEvent)
		void OnDissovle();

public:
	void OnAppear();
	
};
