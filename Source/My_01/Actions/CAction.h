#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/CMontageComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Characters/CEnemy.h"
#include "Actions/Dual/CSpecialEffect.h"
#include "CAction.generated.h"

// 데이터

USTRUCT(BlueprintType)
struct FEquipmentData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere)
		float PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
		FName StartSection;

	UPROPERTY(EditAnywhere)
		bool bCanMove = true;

	UPROPERTY(EditAnywhere)
		bool bPawnControl = true;
};

USTRUCT(BlueprintType)
struct FDoActionData : public FEquipmentData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		bool IsLaunch = false;

	UPROPERTY(EditAnywhere)
		float LaunchValue = 0.0f;

	UPROPERTY(EditAnywhere)
		bool bLast = false; // 마지막 공격

	UPROPERTY(EditAnywhere)
		bool bHitStop = false;  

	UPROPERTY(EditAnywhere)
		float HitStopTime = 0.0f;

	UPROPERTY(EditAnywhere)
		float Power = 0.0f;

	UPROPERTY(EditAnywhere)
		TArray<class UParticleSystem*> HitEffects;

	UPROPERTY(EditAnywhere)
		FTransform HitEffectTransform;

	UPROPERTY(EditAnywhere)
		EHittedType HittedType;
	
	UPROPERTY(EditAnywhere)
		EDeadType DeadType;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UMatineeCameraShake> ShakeClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACThrow> ThrowClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACFake> FakeClass;

	UPROPERTY(EditAnywhere)
		class UNiagaraSystem* HitNiagaraEffect;

};

USTRUCT(BlueprintType)
struct FAirActionData : public FDoActionData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float AirLaunchMeValue;

	UPROPERTY(EditAnywhere)
		float AirLaunchValue;

	UPROPERTY(EditAnywhere)
		bool StartAir;

	UPROPERTY(EditAnywhere)
		bool Air;

	UPROPERTY(EditAnywhere)
		bool EndAir;
};

USTRUCT(BlueprintType)
struct FSkillActionData : public FDoActionData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float AirLaunchValue;
	
	UPROPERTY(EditAnywhere)
		bool Air;

	UPROPERTY(EditAnywhere)
		bool EndAir;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACCreateDecal> DecalClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACSkillEffect> SkillEffectClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACCreateCollisionBox> CollisionBoxClass;

	UPROPERTY(EditAnywhere)
		TArray<class UParticleSystem*> ShotEffects;

	UPROPERTY(EditAnywhere)
		FTransform ShotEffectTransform;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACRange> RangeClass;
};

USTRUCT(BlueprintType)
struct FTakedownActionData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		class UNiagaraSystem* NiagaraEffect;
	
	UPROPERTY(EditAnywhere)
		class UAnimMontage* AttAnimMontage; // 공격자

	UPROPERTY(EditAnywhere)
		float AttPlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
		FName AttStartSection;

	UPROPERTY(EditAnywhere)
		bool AttCanMove = false;

	UPROPERTY(EditAnywhere)
		bool AttPawnControl = true;

	UPROPERTY(EditAnywhere)
		TMap<EEnemyType, class UAnimMontage*> VicMaps;

	UPROPERTY(EditAnywhere)
		float VicPlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
		FName VicStartSection;

	UPROPERTY(EditAnywhere)
		bool VicCanMove = false;

	UPROPERTY(EditAnywhere)
		bool VicPawnControl = true;

};

USTRUCT(BlueprintType)
struct FSpecialActionData : public FEquipmentData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float AirLaunchMeValue;

	UPROPERTY(EditAnywhere)
		float AirLaunchValue;

	UPROPERTY(EditAnywhere)
		bool StartAir;

	UPROPERTY(EditAnywhere)
		bool Air;

	UPROPERTY(EditAnywhere)
		bool EndAir;

	UPROPERTY(EditAnywhere)
		bool bLast = false;

	UPROPERTY(EditAnywhere)
		bool bHitStop = false;

	UPROPERTY(EditAnywhere)
		float HitStopTime = 0.0f;

	UPROPERTY(EditAnywhere)
		float Power = 0.0f;

	UPROPERTY(EditAnywhere)
		TArray<class UParticleSystem*> ParticleEffects;

	UPROPERTY(EditAnywhere)
		FTransform ParticleEffectTransform;

	UPROPERTY(EditAnywhere)
		TArray<class UParticleSystem*> HitEffects;

	UPROPERTY(EditAnywhere)
		FTransform HitEffectTransform;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACSpecialEffect> EffectClass;

	UPROPERTY(EditAnywhere)
		class UNiagaraSystem* NiagaraEffect;
	
	UPROPERTY(EditAnywhere)
		EHittedType HittedType;

	UPROPERTY(EditAnywhere)
		EDeadType DeadType;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UMatineeCameraShake> ShakeClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACThrow> ThrowClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACFake> FakeClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACSkillEffect> SkillEffectClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACCreateCollisionBox> BoxClass;

	UPROPERTY(EditAnywhere)
		bool IsLaunch = false;

	UPROPERTY(EditAnywhere)
		float LaunchValue = 0.0f;

	UPROPERTY(EditAnywhere)
		class UNiagaraSystem* HitNiagaraEffect;
};

USTRUCT(BlueprintType)
struct FGuardAndParrayActionData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* BlockAnimMontage;

	UPROPERTY(EditAnywhere)
		float BlockPlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
		FName BlockStartSection;

	UPROPERTY(EditAnywhere)
		bool BlockCanMove = true;

	UPROPERTY(EditAnywhere)
		bool BlockPawnControl = true;
	
	UPROPERTY(EditAnywhere)
		TSubclassOf<UMatineeCameraShake> ShakeClass;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* ParrayAnimMontage;

	UPROPERTY(EditAnywhere)
		float ParrayPlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
		FName ParrayStartSection;

	UPROPERTY(EditAnywhere)
		bool ParrayCanMove = true;

	UPROPERTY(EditAnywhere)
		bool ParrayPawnControl = true;

	UPROPERTY(EditAnywhere)
		TArray<class UParticleSystem*> ParrayEffects;

	UPROPERTY(EditAnywhere)
		FTransform ParrayEffectTransform;

	UPROPERTY(EditAnywhere)
		TMap<EEnemyType, class UAnimMontage*> ParrayVicMaps;

	UPROPERTY(EditAnywhere)
		float ParrayVicPlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
		FName ParrayVicStartSection;

	UPROPERTY(EditAnywhere)
		float Power;

	UPROPERTY(EditAnywhere)
		EHittedType HittedType;

	UPROPERTY(EditAnywhere)
		EDeadType DeadType;

	UPROPERTY(EditAnywhere)
		bool bHitStop = false;

	UPROPERTY(EditAnywhere)
		float HitStopTime = 0.0f;

	UPROPERTY(EditAnywhere)
		class UNiagaraSystem* HitNiagaraEffect;
};

USTRUCT(BlueprintType)
struct FSummonActionData : public FEquipmentData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float Power;

	UPROPERTY(EditAnywhere)
		EHittedType HittedType;

	UPROPERTY(EditAnywhere)
		EDeadType DeadType;

	UPROPERTY(EditAnywhere)
		bool IsLaunch = false;

	UPROPERTY(EditAnywhere)
		float LaunchValue = 0.0f;

	UPROPERTY(EditAnywhere)
		class UParticleSystem* HitEffect;

	UPROPERTY(EditAnywhere)
		FTransform HitEffectTransform;

	UPROPERTY(EditAnywhere)
		class UNiagaraSystem* NiagaraEffect;

	UPROPERTY(EditAnywhere)
		bool bHitStop = false;

	UPROPERTY(EditAnywhere)
		float HitStopTime = 0.0f;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACCannon> CannonClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACCannonTarget> CannonTargetClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UMatineeCameraShake> ShakeClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACSummonController> SummonControllerClass;

};


UCLASS()
class MY_01_API UCAction : public UObject
{
	GENERATED_BODY()

public:
	friend class UCActionData;

public:
	UFUNCTION(BlueprintCallable)
		FORCEINLINE class ACAttachment* GetAttachment() { return Attachment; }

	UFUNCTION(BlueprintCallable)
		FORCEINLINE class ACEquipment* GetEquipment() { return Equipment; }

	UFUNCTION(BlueprintCallable)
		FORCEINLINE class ACDoAction* GetDoAction() { return DoAction; }

private:
	class ACAttachment* Attachment; // 무기와 충돌체 Attach를 관리하는 클래스
	class ACEquipment* Equipment; // 장착 및 해제를 관리하는 클래스
	class ACDoAction* DoAction; // 전투에 관련된 클래스 
};
