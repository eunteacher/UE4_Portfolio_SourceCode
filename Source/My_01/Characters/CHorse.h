#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CStatusComponent.h"
#include "CHorse.generated.h"

UCLASS()
class MY_01_API ACHorse : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UBoxComponent* Box;

protected:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* DismountLocation;

private:
	// VisibleAnywhere 배치된 상황에서도 편집 가능 
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleDefaultsOnly)
		class UCOptionComponent* Option;


public:
	FORCEINLINE USceneComponent* GetDismountLocation() { return DismountLocation; }
	FORCEINLINE EHorseSpeed GetHorseSpeed() { return SpeedType; }

public:
	ACHorse();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private: // Bind Axis
	void OnHorizontalLook(float Axis);
	void OnVerticalLook(float Axis);

	void OnMoveForward(float Axis);
	void OnMoveRight(float Axis);

	void OnZoomIn();
	void OnZoomOut();

	void OnRun();
	void OffRun();
	
	void OffRiding();

public:
	void SetCanRiding(bool InCanRiding);

private:
	bool CanRiding; // true면 플레이어가 탑승
	
	EHorseSpeed SpeedType;

	class ACharacter* Rider;

};
