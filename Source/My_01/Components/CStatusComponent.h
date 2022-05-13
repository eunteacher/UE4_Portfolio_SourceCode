#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"

UENUM(BlueprintType)
enum class ECharacterSpeed : uint8
{
	Walk, Run, Sprint, Max
};

UENUM(BlueprintType)
enum class EHorseSpeed : uint8
{
	Walk, Sprint, Max
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MY_01_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Speed")
		float Speed[(int32)ECharacterSpeed::Max];

private:
	UPROPERTY(EditAnywhere, Category = "UserSettings")
		float MaxHealth = 200.0f;

	UPROPERTY(EditAnywhere, Category = "UserSettings")
		float MaxStamina = 300.0f;

	UPROPERTY(EditAnywhere, Category = "UserSettings")
		float MaxMana = 300.0f;

	UPROPERTY(EditAnywhere, Category = "UserSettings")
		float WalkSpeed = 400.0f;

	UPROPERTY(EditAnywhere, Category = "UserSettings")
		float RunSpeed = 600.0f;

	UPROPERTY(EditAnywhere, Category = "UserSettings")
		float SprintSpeed = 800.0f;

	UPROPERTY(EditAnywhere, Category = "UserSettings")
		float Horse_WalkSpeed = 160.0f;

	UPROPERTY(EditAnywhere, Category = "UserSettings")
		float Horse_SprintSpeed = 1000.0f;

public:
	FORCEINLINE float GetHealth() { return Health; }
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }

	FORCEINLINE float GetStamina() { return Stamina; }
	FORCEINLINE float GetMaxStamina() { return MaxStamina; }

	FORCEINLINE float GetMana() { return Mana; }
	FORCEINLINE float GetMaxMana() { return MaxMana; }

	FORCEINLINE bool GetCanMove() { return bCanMove == true; }

	FORCEINLINE float GetWalkSpeed() { return WalkSpeed; }
	FORCEINLINE float GetRunSpeed() { return RunSpeed; }
	FORCEINLINE float GetSprintSpeed() { return SprintSpeed; }

	FORCEINLINE float GetHorse_WalkSpeed() { return Horse_WalkSpeed; }
	FORCEINLINE float GetHorse_SprintSpeed() { return Horse_SprintSpeed; }

public:	
	UCStatusComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	void SetMove();
	void SetStop();

	void SetSpeed(ECharacterSpeed InSpeed);
	void SetSpeed(EHorseSpeed InSpeed);

	void AddHealth(float InAmount);
	void SubHealth(float InAmount);

	void AddStamina(float InAmount);
	void SubStamina(float InAmount);

	void AddMana(float InAmount);
	void SubMana(float InAmount);
		
private:
	float Health;
	float Stamina;
	float Mana;

	bool bCanMove;
	

};
