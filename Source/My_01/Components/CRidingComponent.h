#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CRidingComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MY_01_API UCRidingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE FVector GetMountLocation() { return MountLocation; }
	FORCEINLINE FRotator GetMountRotation() { return MountRotation; }
	FORCEINLINE FVector GetMountRightVec() { return MountRightVec; }
	FORCEINLINE FVector GetMountLeftVec() { return MountLeftVec; }

	FORCEINLINE bool GetPossibleRiding() { return bPossibleRiding; }
	FORCEINLINE bool GetIsMountRight() { return IsMountRight; }

	FORCEINLINE class ACHorse* GetHorse() { return Horse; }

public:	
	UCRidingComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetMountValue(FVector InMountLocation, FRotator InMountRotation);
	void SetMountDirectionVec(FVector InMountRightVec, FVector InMountLeftVec);
	void SetHorse(class ACHorse* InHorse);

public:
	void OnRiding();
	void OffRiding();

	void SetOnRiding();
	void SetOffRiding();

	void SetMountRight();
	void SetMountLeft();
	
	void MoveMount();
	void EndMount();

	void MoveDismount();
	void EndDismount();

	UFUNCTION()
		void StopMoveMount();

	UFUNCTION()
		void StopMoveDismount();

private:
	class ACharacter* OwnerCharacter;

	bool bPossibleRiding;
	bool IsMountRight; // true -> right , false -> left

	FVector MountLocation;
	FRotator MountRotation;
	FVector MountRightVec;
	FVector MountLeftVec;

	class ACHorse* Horse;

};
