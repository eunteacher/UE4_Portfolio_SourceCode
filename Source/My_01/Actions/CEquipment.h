#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actions/CAction.h"
#include "CEquipment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUnEquipmentDelegate);

UCLASS()
class MY_01_API ACEquipment : public AActor
{
	GENERATED_BODY()

public:
	FORCEINLINE const bool* GetEquipped() { return &bEquipped; }
	FORCEINLINE void SetEquipData(FEquipmentData InData) { EquipData = InData; }

public:
	ACEquipment();

public:
	void Equip();
	void Begin_Equip();
	void End_Equip();

	void UnEquip();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable)
		FEquipmentDelegate OnEquipmentDelegate;

	UPROPERTY(BlueprintAssignable)
		FUnEquipmentDelegate OnUnEquipmentDelegate;

protected:
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly)
		class UCStateComponent* State;

	UPROPERTY(BlueprintReadOnly)
		class UCStatusComponent* Status;

	UPROPERTY(BlueprintReadOnly)
		class UCActionComponent* Action;

	
private:
	bool bEquipped;
	FEquipmentData EquipData;

};
