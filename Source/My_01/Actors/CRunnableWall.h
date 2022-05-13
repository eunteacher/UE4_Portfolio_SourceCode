#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CRunnableWall.generated.h"



UCLASS()
class MY_01_API ACRunnableWall : public AActor
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditAnywhere)
		bool bMove = false;

	UPROPERTY(EditAnywhere)
		float Speed = 100.0f;

	UPROPERTY(EditAnywhere)
		float StopTime = 100.0f;


private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleDefaultsOnly)
		class UBoxComponent* BoxLeft;

	UPROPERTY(VisibleDefaultsOnly)
		class UBoxComponent* BoxRight;

public:	
	ACRunnableWall();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION()
		void OnComponentBeginOverlapBox1(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void OnComponentEndOverlapBox1(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION()
		void OnComponentBeginOverlapBox2(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void OnComponentEndOverlapBox2(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


private:
	float Total;
	bool bStop = false;
	bool bMoveLeft = true;
	bool bMoveRight = false;
};
