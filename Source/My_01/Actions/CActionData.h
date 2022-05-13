#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CAction.h"
#include "CActionData.generated.h"

UCLASS()
class MY_01_API UCActionData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class ACAttachment> AttachmentClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class ACEquipment> EquipmentClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FEquipmentData EquipmentData;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class ACDoAction> DoActionClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<FDoActionData> DoActionDatas;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<FAirActionData> AirActionDatas;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<FSkillActionData> SkillActionDatas;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<FTakedownActionData> TakedownActionDatas;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<FSpecialActionData> SpecialActionDatas;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<FGuardAndParrayActionData> GuardAndParrayActionDatas;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<FSummonActionData> SummonDatas;

public:
	void BeginPlay(class ACharacter* InOwnerCharacter, class UCAction** OutAction);
	
};
