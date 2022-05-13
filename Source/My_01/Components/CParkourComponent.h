#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/TimelineComponent.h"
#include "Characters/IWallRun.h"
#include "CParkourComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MY_01_API UCParkourComponent : public UActorComponent, public IIWallRun
{
	GENERATED_BODY()


protected:
	UPROPERTY(EditAnywhere, Category = "Parkour")
		TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::ForOneFrame;

	UPROPERTY(EditAnywhere, Category = "Parkour")
		TEnumAsByte<ETraceTypeQuery> TraceType = ETraceTypeQuery::TraceTypeQuery3;

	UPROPERTY(EditAnywhere, Category = "Parkour")
		FName SocketName = "PelvisSocket";

protected:
	UPROPERTY(EditAnywhere, Category = "UserSetting")
		float HeightTraceLength = 200.0f;

	UPROPERTY(EditAnywhere, Category = "UserSetting")
		float MinPelvisSubtractWallHeight = -60.0f;

	UPROPERTY(EditAnywhere, Category = "UserSetting")
		float MaxPelvisSubtractWallHeight = -40.0f;

	UPROPERTY(EditAnywhere, Category = "UserSetting")
		float HeightOffset = -110.0f;
	
	UPROPERTY(EditAnywhere, Category = "UserSetting")
		float HeightLimit = 40.0f;

	UPROPERTY(EditAnywhere, Category = "UserSetting")
		float BehindOffset = 230.0f;

	UPROPERTY(EditAnywhere, Category = "UserSetting")
		float DistacneOffset = 30.0f;

	UPROPERTY(EditAnywhere, Category = "UserSetting")
		float JumpOverDistacneOffset = 90.0f;

	UPROPERTY(EditAnywhere, Category = "UserSetting")
		float JumpOverHeight = 60.0f;



public:
	FORCEINLINE bool IsHanging() { return Hanging; }
	FORCEINLINE bool GetCanMoveRight() { return CanMoveRight; }
	FORCEINLINE bool GetMovingRight() { return MovingRight; }
	FORCEINLINE bool GetCanMoveLeft() { return CanMoveLeft; }
	FORCEINLINE bool GetMovingLeft() { return MovingLeft; }
	FORCEINLINE bool GetCanJumpLeft() { return CanJumpLeft; }
	FORCEINLINE bool GetCanJumpRight() { return CanJumpRight; }
	FORCEINLINE bool GetCanJumpUp() { return CanJumpUp; }

	FORCEINLINE bool CanGoUpWall() { return bGoUpWall; }
	FORCEINLINE bool CanJumpOver() { return bJumpOver; }

	FORCEINLINE bool CanWallRun() { return bCanWallRun; }

public:	
	UCParkourComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
private:
	void ForwardTrace();
	void HeightTrace();
	
	FVector RightTrace();
	FVector LeftTrace();

	void JumpLeftTrace();
	void JumpRightTrace();
	void JumpForwardTrace(FVector InStart);
	void JumpHeightTrace(FVector InStart);
	
	void TurnLeftTrace(FVector InStart);
	void TurnRightTrace(FVector InStart);

	void JumpUpTrace();
	void JumpUpForwardTrace(FVector InStart, FVector InHitLoaction);

	void JumpOverTrace(FVector InStart);
	void BehindTrace(FVector InStart);

	bool HipToLedge();

	UFUNCTION()
		void GrabLedge();

	void MoveInLedge();
	void MoveLeftRigt();

	FVector MoveToLocation();
	FRotator LookAtWall();

	UFUNCTION(BlueprintCallable)
		void MoveToGrabLedge();

	UFUNCTION()
		void CallTimer();

	UFUNCTION(BlueprintCallable)
		void MoveToBehindLocation();

	UFUNCTION(BlueprintCallable)
		void Restore();

	UFUNCTION(BlueprintCallable)
		void LastMoveTo();

public:
	void OffParkour();
	
	void ClimbLedge();

	void EndClimbLedge();
	void EndJumpLeftRight();

	float RightAxis();

	void JumpLeftGrabLedge();
	void JumpRightGrabLedge();

	void TurnParkour();

	void JumpUpLedge();
	void EndJumpUpLedge();

	void TurnedBack();
	void TurnedForward();
	void TurnedJump();

	void GoUpWall();
	void JumpOver();

	void WallRun();

	UFUNCTION()
		void StopMoveToWallRun();

	virtual void SetRunnableWall(bool Enable, bool IsRight, FVector Target, FTransform Start) override;

private:
	class ACharacter* OwnerCharacter;
	class UCStateComponent* State;

	FVector WallLocation = FVector(0, 0, 0);
	FVector WallNormal = FVector(0, 0, 0);
	FVector WallHeight = FVector(0, 0, 0);
	FVector WallBehindLocation = FVector(0, 0, 0);
	FVector WallJumOverHeight = FVector(0, 0, 0);
	FVector WallJumOverNormal = FVector(0, 0, 0);


	bool bClimbLedge = false;
	bool Hanging = false;

	bool CanMoveRight;
	bool MovingRight;
	bool CanMoveLeft;
	bool MovingLeft;

	bool CanJumpLeft;
	bool CanJumpRight;

	bool Jumping;

	bool CanTurnLeft;
	bool CanTurnRight;

	bool CanJumpUp;

	bool TurnBack;

	bool bGoUpWall;
	bool bJumpOver = false;

	
	FVector HangingWall = FVector(0, 0, 0);
	FVector JumpingWall = FVector(0, 0, 0);

	bool bCanWallRun;
	FVector TargetLocation;
	FTransform StartTransform;
	bool bRight;

};
