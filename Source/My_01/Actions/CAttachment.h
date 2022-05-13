#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAttachment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttachmentBeginOverlap, class ACharacter*, InAttacker, class AActor*, InAttackCauser, class ACharacter*, InOtherCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttachmentEndOverlap, class ACharacter*, InAttacker, class AActor*, InAttackCauser, class ACharacter*, InOtherCharacter);

UCLASS()
class MY_01_API ACAttachment : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;

public:	
	ACAttachment();

protected:
	virtual void BeginPlay() override;

public:
	void OnCollision(); // �浹ü �ѱ�
	void OffCollision(); // �浹ü ����

	UFUNCTION(BlueprintImplementableEvent)
		void OnEquip(); // ���� ����

	UFUNCTION(BlueprintImplementableEvent)
		void OnUnEquip(); // ���� ����

public:
	// ���Ⱑ Bow�� ��� 
	UFUNCTION(BlueprintImplementableEvent)
		void OnOverDraw();

	UFUNCTION(BlueprintImplementableEvent)
		void OnOverDraw_Loop();

	UFUNCTION(BlueprintImplementableEvent)
		void OnFire();

	UFUNCTION(BlueprintImplementableEvent)
		void OnBowIdle();

public:
	UFUNCTION(BlueprintImplementableEvent)
		void OnAppear();

	UFUNCTION(BlueprintImplementableEvent)
		void OnDisappear();
	
	UPROPERTY(BlueprintAssignable)
		FAttachmentBeginOverlap OnAttachmentBeginOverlap;

	UPROPERTY(BlueprintAssignable)
		FAttachmentEndOverlap OnAttachmentEndOverlap;

protected:
	UFUNCTION(BlueprintCallable)
		void AttachTo(FName InSocketName);

private:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly)
		class UCStateComponent* State;

	UPROPERTY(BlueprintReadOnly)
		class UCStatusComponent* Status;

private:
	TArray<class UShapeComponent*> ShapeComponents; // �浹ü 
};
