#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "Components/CStateComponent.h"
#include "Components/CActionComponent.h"
#include "Widgets/CUserWidget_StatusBar.h"
#include "Widgets/CUserWidget_SkillIcon.h"
#include "CPlayer.generated.h"

UCLASS()
class MY_01_API ACPlayer : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Team")
		uint8 TeamID = 0;
	
public:
	UPROPERTY(EditAnywhere)
		class UNiagaraSystem* NiagaraEffect;

	UPROPERTY(EditAnywhere)
		float MaxCooldown_SkillAction = 3.0f;
	
	UPROPERTY(EditAnywhere)
		float MaxCooldown_SpecialAction = 5.0f;

	UPROPERTY(EditAnywhere)
		float ManaConsumeValue = 100.0f;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UCUserWidget_StatusBar> StatusBarWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "CamearShake")
		TSubclassOf<class UMatineeCameraShake> Shake;

private:
	// VisibleAnywhere 배치된 상황에서도 편집 가능 
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;

private:
	// VisibleDefaultsOnly 외부에서 건들지않는 컴포넌트
	UPROPERTY(VisibleDefaultsOnly)
		class UCOptionComponent* Option;
	
	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;

	UPROPERTY(VisibleDefaultsOnly)
		class UCActionComponent* Action;

	UPROPERTY(VisibleDefaultsOnly)
		class UCFeetComponent* Feet;

	UPROPERTY(VisibleDefaultsOnly)
		class UCParkourComponent* Parkour;

	UPROPERTY(VisibleDefaultsOnly)
		class UCMontageComponent* Montage;

	UPROPERTY(VisibleDefaultsOnly)
		class UCGrappleComponent* Grapple;

	UPROPERTY(VisibleDefaultsOnly)
		class UCTargetComponent* Target;

	UPROPERTY(VisibleDefaultsOnly)
		class UCRidingComponent* Riding;

public:
	UFUNCTION(BlueprintCallable)
		FORCEINLINE class UCUserWidget_StatusBar* GetStatusBar() { return StatusBar; }

public:
	ACPlayer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	FGenericTeamId GetGenericTeamId() const override;
	void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;

	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* PawnInstigator, AActor* DamageCauser) override;
	
private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

	UFUNCTION()
		void OnMovementTypeChanged(EMovementType InPrevType, EMovementType InNewType);

	UFUNCTION()
		void OnActionTypeChanged(EActionType InPrevType, EActionType InNewType);

public:
	FORCEINLINE bool GetAirSwap() { return bAirSwap; }
	FORCEINLINE void DisableAirSwap() { bAirSwap = false; }

private: // Bind Axis
	void OnHorizontalLook(float Axis);
	void OnVerticalLook(float Axis);

	void OnMoveForward(float Axis);
	void OnMoveRight(float Axis);

	void OnZoomIn();
	void OnZoomOut();

public: // Bind Action
	void OnRun();
	void OffRun();

	void OnSprint();
	void OffSprint();

	void OnJump();
	void OffJump();

	void OnTurnBack();
	void OnTurnParkour();
	void OnTurnForward();
	void OnTurnJump();

	void OnDash();

	void OnRiding();

private:
	UFUNCTION()
		void OnDual();

	UFUNCTION()
		void OnArcher();

	UFUNCTION()
		void OnSpear();

	void OnDoAction();
	void OnAirAction();
	void OnSkillAction();
	void OnTakedownAction();
	void OnSpecialAction();

	void OnAim();
	void OffAim();

	void Hitted();

	void Dead();

	void OnGrapple();

	void OnCrouch();
	void OffCrouch();

	void Walking();

	void Flying();

	void Crouching();

	void OnTarget();

	void OnGuard();

	void OnParry();

private:
	UFUNCTION()
		void SetJumpCount();

public:
	UFUNCTION(BlueprintImplementableEvent)
		void OnSequence_Dual();

	UFUNCTION(BlueprintImplementableEvent)
		void OffSequence_Dual();

	UFUNCTION(BlueprintImplementableEvent)
		void OnSequence_Archer();

	UFUNCTION(BlueprintImplementableEvent)
		void OffSequence_Archer();

	UFUNCTION(BlueprintImplementableEvent)
		void OnSequence_Unarmed();

	UFUNCTION(BlueprintImplementableEvent)
		void OffSequence_Unarmed();

	UFUNCTION(BlueprintCallable)
		void StartIntro();

	UFUNCTION(BlueprintCallable)
		void EndIntro();


private:
	UFUNCTION()
		void EndDash();

private:
	class UCUserWidget_StatusBar* StatusBar;

	bool bRun;

	class AController* DamageInstigator;

	int32 JumpCount;
	bool bResetJumpCount;

	bool bSequence;

	bool bCooldown;
	
	bool bCooldown_Skill;
	bool bCooldown_Special;

	float Cooldown_SkillAction;
	float Cooldown_SpecialAction;

	bool bAirSwap;
};
