#pragma once

#include "CoreMinimal.h"
#include "Characters/CEnemy_AI.h"
#include "CEnemy_AI_Archer.generated.h"

UCLASS()
class MY_01_API ACEnemy_AI_Archer : public ACEnemy_AI
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* HealthWidget;

public:
	ACEnemy_AI_Archer();

private:
	void BeginPlay() override;

public:
	void Tick(float DeltaTime) override;

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
	void Running() override;
	void Flying() override;
	void FindTarget() override;

public:
	FVector GetPlayerLocation() { return PlayerLocation; }

public:
	void Avoid();

private:
	FVector PlayerLocation;

};
