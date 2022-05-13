#pragma once

#include "CoreMinimal.h"
#include "Characters/CEnemy_AI.h"
#include "CEnemy_AI_GirlKnight.generated.h"

UCLASS()
class MY_01_API ACEnemy_AI_GirlKnight : public ACEnemy_AI
{
	GENERATED_BODY()

public:
	ACEnemy_AI_GirlKnight();

private:
	void BeginPlay() override;

public:
	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* PawnInstigator, AActor* DamageCauser) override;
	
	UFUNCTION(BlueprintCallable)
		void SetStatusBar(class UCUserWidget_StatusBar* InStatusBar) { StatusBar = InStatusBar; }

private:
	void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType) override;
	void OnMovementTypeChanged(EMovementType InPrevType, EMovementType InNewType) override;

	void Hitted() override;
	void AirHitted() override;
	void Dead() override;

	void Walking() override;
	void Flying() override;

private:
	class ACPlayer* Player;
	class UCUserWidget_StatusBar* StatusBar;

};
