#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "Components/CActionComponent.h"
#include "CDoAction_Archer.generated.h"

UENUM(BlueprintType)
enum class EArcherStateType : uint8
{
	OverDraw, Fire
};

UCLASS()
class MY_01_API ACDoAction_Archer : public ACDoAction
{
	GENERATED_BODY()

public:
	void EnableFire() { IsFire = true; }
	
public:
	ACDoAction_Archer();

private:
	void BeginPlay() override;

public:
	void Tick(float DeltaTime) override;

public:
	void DoAction() override;
	void Begin_DoAction() override;
	void End_DoAction() override;

	void SkillAction() override;
	void Begin_SkillAction() override;
	void End_SkillAction() override;

	void OnAim() override;
	void OffAim() override;

	void SpecialAction() override;
	void Begin_SpecialAction() override;
	void End_SpecialAction() override;

	void TakedownAction() override;

	void Reset() override;

private:
	UFUNCTION() // 투사체 충돌체 검사
		void OnThrowBeginOverlap(FHitResult InHitResult, class ACharacter* InOtherCharacter);

	UFUNCTION() 
		void OnCollisionBoxBeginOverlap(FHitResult InHitResult, class ACharacter* InOtherCharacter);

	UFUNCTION() // 스폐셜 공격 충돌체 검사
		void OnThrowSpecialBeginOverlap(FHitResult InHitResult, class ACharacter* InOtherCharacter);

	UFUNCTION()
		void OnRangeEndOverlap(class AActor* InOtherActor);

	UFUNCTION()
		void OnRangeBeginOverlap(class AActor* InOtherActor);

	UFUNCTION()
		void ShowFakeArrow();

	UFUNCTION()
		void AirJump();

	UFUNCTION()
		void TakedownFinish();

	UFUNCTION()
		void ArcherSkill_SpawnHitEffect();

private:
	void TakedownShotArrow();

private:
	void WarpArrow_Archer(class ACharacter* InOther);

	bool GetCursorLocationAndRotation(FVector& OutLocation, FRotator& OutRotator);

	void DoActionReadData(const FDoActionData& InData, class ACharacter* InOtherCharacter);
	void SpecialActionReadData(const FSpecialActionData& InData, class ACharacter* InOtherCharacter);

public:
	void ArcherSkill_SpawnShotEffect();

	void ArcherSpecial_ShotArrow();

	void CreateSkillEffect();
private:
	UFUNCTION()
		void OnActionTypeChanged(EActionType InPrevType, EActionType InNewType);

private:
	class UCAim* Aim;
	class ACFake* FakeArrow;

	EArcherStateType Type;

	bool IsFire;

	bool bAirAction;
	bool bAirZoom;
	
	//class UDecalComponent* Decal;
	//bool bReady;
	FVector HitLocation;

	bool bAirShot;

	class ACCreateDecal* Decal;

	TArray<class ACCreateCollisionBox*> CollisionBoxs;
	TArray<class ACharacter*> HitCharacter_Archer_Skill;

	class ACFake* FakeArrow_Special;

	float TotalTime;

	TArray<FRotator> ThrowRotators;

	class ACSkillEffect* SpecialEffect;

	class ACRange* SkillRange;

	bool bWarning;
};
