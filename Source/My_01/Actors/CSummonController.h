#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSummonController.generated.h"

UCLASS()
class MY_01_API ACSummonController : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Summon")
		TArray<class ACEnemy_AI_Summon_GirlKngiht*> Summons;
	
	UPROPERTY(EditDefaultsOnly, Category = "Summon")
		TSubclassOf<class ACEnemy_AI_Summon_GirlKngiht> SummonClass;

	UPROPERTY(EditDefaultsOnly, Category = "Particle")
		class UParticleSystem* Particle;

public:
	void SetSummoner(class ACharacter* InSummoner);

public:	
	ACSummonController();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	void OnSummon();

private:
	class ACharacter* Summoner;

};
