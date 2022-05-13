#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_StatusBar.generated.h"

UCLASS()
class MY_01_API UCUserWidget_StatusBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateHealthBar(float Health, float MaxHealth);

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateStaminaBar(float Stamina, float MaxStamina);

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateManaBar(float Mana, float MaxMana);

	UFUNCTION(BlueprintImplementableEvent)
		void OnTakedown();

	UFUNCTION(BlueprintImplementableEvent)
		void OffTakedown();

	UFUNCTION(BlueprintImplementableEvent)
		void OnParry();

	UFUNCTION(BlueprintImplementableEvent)
		void OffParry();

	UFUNCTION(BlueprintImplementableEvent)
		void OnDualMode();

	UFUNCTION(BlueprintImplementableEvent)
		void OnArcherMode();

	UFUNCTION(BlueprintImplementableEvent)
		void OnUnarmedMode();

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateCooldown_Dual_Skill(float MaxCooldownValue, float CooldownValue, bool bReset);

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateCooldown_Dual_Special(float MaxCooldownValue, float CooldownValue, bool bReset);

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateCooldown_Archer_Skill(float MaxCooldownValue, float CooldownValue, bool bReset);

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateCooldown_Archer_Special(float MaxCooldownValue, float CooldownValue, bool bReset);

	UFUNCTION(BlueprintImplementableEvent)
		void OnCantUse_ArcherSkill();

	UFUNCTION(BlueprintImplementableEvent)
		void OnCantUse_ArcherSpecial();

	UFUNCTION(BlueprintImplementableEvent)
		void OnCantUse_DualSkill();

	UFUNCTION(BlueprintImplementableEvent)
		void OnCantUse_DualSpecial();

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateHealthBar_Boss(float Health, float MaxHealth);

	UFUNCTION(BlueprintImplementableEvent)
		void AppearHealthBar_Boss();

	UFUNCTION(BlueprintImplementableEvent)
		void OnLackMana();

	UFUNCTION(BlueprintImplementableEvent)
		void OnRiding();

	UFUNCTION(BlueprintImplementableEvent)
		void OffRiding();

	UFUNCTION(BlueprintImplementableEvent)
		void DisappearHealthBar_Boss();

};
