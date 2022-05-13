#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/CStateComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CFeetComponent.h"
#include "IParkour.h"
#include "CAnimInstance.generated.h"

UCLASS()
class MY_01_API UCAnimInstance : public UAnimInstance, public IIParkour
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Speed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Direction;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool InAir;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool Hanging;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool MoveRight;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool CanMoveRight;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool MoveLeft;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool CanMoveLeft;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool TurnBack;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		EActionType ActionType;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		EStateType StateType;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		EMovementType MovementType;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FFeetData Feet;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool FeetStop;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Pitch;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool Aiming;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float HorseSpeed;

public:
	FORCEINLINE void SetAiming(bool InAiming) { Aiming = InAiming; }
	FORCEINLINE void SetHorseSpeed(float InSpeed) { HorseSpeed = InSpeed; }

	FORCEINLINE float GetSpeed() { return Speed; }

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void CanGrab(bool bCanGrabPI) override;
	virtual void ClimbLedge(bool bIsClimbingPI) override;
	virtual void MoveLeftRight(float InAxis) override;
	virtual void JumpLeft() override;
	virtual void JumpRight() override;
	virtual void TurnLeft() override;
	virtual void TurnRight() override;
	virtual void JumpUp() override;
	virtual void TurnedBack(bool InTurnBack) override;
	virtual void GoUpWall() override;
	virtual void JumpOver() override;
	virtual void WallRunRight() override;
	virtual void WallRunLeft() override;

private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);
	
	UFUNCTION()
		void OnMovementTypeChanged(EMovementType InPrevType, EMovementType InNewType);

	UFUNCTION()
		void OnActionTypeChanged(EActionType InPrevType, EActionType InNewType);


private:
	class ACharacter* OwnerCharacter;

	
};
