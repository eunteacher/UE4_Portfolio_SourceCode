#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CFeetComponent.generated.h"

USTRUCT(BlueprintType)
struct FFeetData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector PelvisDistance;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector LeftDistance;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector RightDistance;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FRotator LeftRotation;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FRotator RightRotation;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MY_01_API UCFeetComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "InverseKinemetics")
		TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::ForOneFrame;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "InverseKinemetics")
		float TraceDistance = 55.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "InverseKinemetics")
		float OffsetDistace = 5.0f; // 발과 땅 사이에 간격

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "InverseKinemetics")
		float InterpSpeed = 13.0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "InverseKinemetics")
		FName LeftSocket = "foot_l";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "InverseKinemetics")
		FName RightSocket = "foot_r";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "InverseKinemetics")
		float PelvisInterpSpeed = 17.0f;

public:
	FORCEINLINE const FFeetData& GetData() { return Data; }
	FORCEINLINE bool GetStop() { return bStop; }

public:	
	UCFeetComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void Trace(FName InSocket, float& OutDistance, FRotator& OutRotation);

public:
	void SetStop();
	void SetUse();

private:
	class ACharacter* OwnerCharacter;
	class UCStateComponent* State;

	FFeetData Data;
	float ScaledHalfHeight;

	bool bStop = false;
		
};
