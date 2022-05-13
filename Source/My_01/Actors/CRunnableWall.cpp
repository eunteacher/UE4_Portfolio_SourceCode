#include "Actors/CRunnableWall.h"
#include "Global.h"

#include "Characters/IWallRun.h"

#include "GameFramework/Character.h"

#include "Components/ShapeComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Components/CParkourComponent.h"

ACRunnableWall::ACRunnableWall()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene");
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &StaticMesh, "StaticMesh", Scene);
	CHelpers::CreateComponent<UBoxComponent>(this, &BoxLeft, "BoxLeft", Scene);
	CHelpers::CreateComponent<UBoxComponent>(this, &BoxRight, "BoxRight", Scene);

}

void ACRunnableWall::BeginPlay()
{
	Super::BeginPlay();
	if (BoxLeft != NULL && BoxRight != NULL)
	{
		BoxLeft->OnComponentBeginOverlap.AddDynamic(this, &ACRunnableWall::OnComponentBeginOverlapBox1);
		BoxRight->OnComponentBeginOverlap.AddDynamic(this, &ACRunnableWall::OnComponentBeginOverlapBox2);
		BoxLeft->OnComponentEndOverlap.AddDynamic(this, &ACRunnableWall::OnComponentEndOverlapBox1);
		BoxRight->OnComponentEndOverlap.AddDynamic(this, &ACRunnableWall::OnComponentEndOverlapBox2);
	}
}

void ACRunnableWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bMove == true)
	{
		FVector position = GetActorLocation();
		Total += Speed * DeltaTime;
		//CLog::Log(Total);
		if (Total > 750.0f)
		{
			bStop = true;
			Total = 0.0f;
		}
		else
		{
			if (bStop == false)
			{
				if (bMoveLeft == true)
				{
					position.X += (Speed * DeltaTime);
					SetActorLocation(position);
				}

				if (bMoveRight == true)
				{
					position.X -= (Speed * DeltaTime);
					SetActorLocation(position);
				}
			}
		}

		if (bStop == true)
		{
			Total += DeltaTime;

			if (Total > StopTime)
			{
				//CLog::Log(Total);
				bStop = false;
				Total = 0.0f;

				if (bMoveLeft == false)
				{
					bMoveRight = false;
					bMoveLeft = true;

					return;
				}

				if (bMoveRight == false)
				{
					bMoveLeft = false;
					bMoveRight = true;

					return;
				}
			}
		}
	}
}

void ACRunnableWall::OnComponentBeginOverlapBox1(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	CheckNull(player);

	UCParkourComponent* parkour = CHelpers::GetComponent<UCParkourComponent>(player);
	CheckNull(parkour);

	IIWallRun* wallRun = Cast<IIWallRun>(parkour);
	CheckNull(wallRun);

	bool bEnable = true;
	bool bRight = false;
	FVector target = BoxRight->GetComponentLocation();
	FTransform start = BoxLeft->GetComponentTransform();

	wallRun->SetRunnableWall(bEnable, bRight, target, start);
}

void ACRunnableWall::OnComponentEndOverlapBox1(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	CheckNull(player);

	UCParkourComponent* parkour = CHelpers::GetComponent<UCParkourComponent>(player);
	CheckNull(parkour);

	IIWallRun* wallRun = Cast<IIWallRun>(parkour);
	CheckNull(wallRun);

	bool bEnable = false;
	bool bRight = false;
	FVector target = FVector(0, 0, 0);
	FTransform start;

	wallRun->SetRunnableWall(bEnable, bRight, target, start);
}

void ACRunnableWall::OnComponentBeginOverlapBox2(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	CheckNull(player);

	UCParkourComponent* parkour = CHelpers::GetComponent<UCParkourComponent>(player);
	CheckNull(parkour);

	IIWallRun* wallRun = Cast<IIWallRun>(parkour);
	CheckNull(wallRun);

	bool bEnable = true;
	bool bRight = true;
	FVector target = BoxLeft->GetComponentLocation();
	FTransform start = BoxRight->GetComponentTransform();

	wallRun->SetRunnableWall(bEnable, bRight, target, start);
}

void ACRunnableWall::OnComponentEndOverlapBox2(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	CheckNull(player);

	UCParkourComponent* parkour = CHelpers::GetComponent<UCParkourComponent>(player);
	CheckNull(parkour);

	IIWallRun* wallRun = Cast<IIWallRun>(parkour);
	CheckNull(wallRun);

	bool bEnable = false;
	bool bRight = false;
	FVector target = FVector(0, 0, 0);
	FTransform start;

	wallRun->SetRunnableWall(bEnable, bRight, target, start);
}
