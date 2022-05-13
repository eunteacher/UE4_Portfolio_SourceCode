#include "Characters/CHorse.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Components/ShapeComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"

#include "Components/COptionComponent.h"
#include "Components/CRidingComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"

#include "Camera/CameraComponent.h"

#include "Animation/AnimInstance.h"

#include "Characters/CAnimInstance.h"
#include "Characters/CPlayer.h"

#include "Widgets/CUserWidget_StatusBar.h"

ACHorse::ACHorse()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<UBoxComponent>(this, &Box, "Box", GetMesh());
	Box->SetRelativeLocation(FVector(0.0f, 50.0f, 60.0f));
	Box->SetRelativeScale3D(FVector(2.0f, 1.0f, 2.0f));

	// Mesh 기본값 설정
	GetMesh()->SetRelativeLocation(FVector(-50.0f, 0.0f, -80.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/HorseAnimsetPro/Meshes/Horses/SK_Horse.SK_Horse'");
	GetMesh()->SetSkeletalMesh(mesh);

	CHelpers::CreateComponent<USceneComponent>(this, &DismountLocation, "DismountLocation", GetMesh());
	DismountLocation->SetRelativeLocation(FVector(80.0f, 0.0f, 0.0f));
	DismountLocation->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetMesh());

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

	bUseControllerRotationYaw = false;

	SpringArm->SetRelativeLocation(FVector(0, 0, 140));
	SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
	SpringArm->TargetArmLength = 500.0f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bDoCollisionTest = true;

	Camera->SetRelativeLocation(FVector(0.0f, 0.0f, 140.0f));
	Camera->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));

	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);
	GetCharacterMovement()->bOrientRotationToMovement = true;

	Option = CreateDefaultSubobject<UCOptionComponent>("Option");
	Status = CreateDefaultSubobject<UCStatusComponent>("Status");
}

void ACHorse::BeginPlay()
{
	Super::BeginPlay();

	CanRiding = false;

	TArray<UShapeComponent*> shapes;
	GetComponents<UShapeComponent>(shapes);
	for (UShapeComponent* shape : shapes)
	{
		//CLog::Print(shape->GetName());
		shape->OnComponentBeginOverlap.AddDynamic(this, &ACHorse::OnComponentBeginOverlap);
		shape->OnComponentEndOverlap.AddDynamic(this, &ACHorse::OnComponentEndOverlap);
	}

	SpeedType = EHorseSpeed::Walk;
	Status->SetSpeed(SpeedType);
}

void ACHorse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CanRiding == true && Rider != NULL)
	{
		UCAnimInstance* animInstance = Cast<UCAnimInstance>(GetMesh()->GetAnimInstance());
		CheckNull(animInstance);

		float speed = animInstance->GetSpeed();

		UCAnimInstance* riderAnimInstance = Cast<UCAnimInstance>(Rider->GetMesh()->GetAnimInstance());
		CheckNull(riderAnimInstance);

		riderAnimInstance->SetHorseSpeed(speed);
	}
}

void ACHorse::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("HorizontalLook", this, &ACHorse::OnHorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", this, &ACHorse::OnVerticalLook);
	PlayerInputComponent->BindAxis("MoveForward", this, &ACHorse::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACHorse::OnMoveRight);
	
	PlayerInputComponent->BindAction("ZoomIn", EInputEvent::IE_Pressed, this, &ACHorse::OnZoomIn);
	PlayerInputComponent->BindAction("ZoomOut", EInputEvent::IE_Pressed, this, &ACHorse::OnZoomOut);
	PlayerInputComponent->BindAction("Riding", EInputEvent::IE_Pressed, this, &::ACHorse::OffRiding);
	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Pressed, this, &ACHorse::OnRun);
	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Released, this, &ACHorse::OffRun);
}

void ACHorse::OnHorizontalLook(float Axis)
{
	CheckFalse(CanRiding);

	float rate = Option->GetHorizontalLookRate();
	AddControllerYawInput(Axis * rate * GetWorld()->GetDeltaSeconds());
}

void ACHorse::OnVerticalLook(float Axis)
{
	CheckFalse(CanRiding);

	float rate = Option->GetVerticalLookRate();
	AddControllerPitchInput(Axis * rate * GetWorld()->GetDeltaSeconds());
}

void ACHorse::OnMoveForward(float Axis)
{
	CheckFalse(CanRiding);

	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector();

	AddMovementInput(direction, Axis);
}

void ACHorse::OnMoveRight(float Axis)
{
	CheckFalse(CanRiding);

	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector();

	AddMovementInput(direction, Axis);
}

void ACHorse::OnZoomIn()
{
	CheckFalse(CanRiding);

	float targetArmLength = SpringArm->TargetArmLength;
	targetArmLength -= 50.0f;
	SpringArm->TargetArmLength = UKismetMathLibrary::Clamp(targetArmLength, 500, 1000);
	float cameraHeight = Camera->GetRelativeLocation().Z;
	cameraHeight -= 10.0f;
	Camera->SetRelativeLocation(FVector(0.0f, 0.0f, UKismetMathLibrary::Clamp(cameraHeight, 140, 240)));
}

void ACHorse::OnZoomOut()
{
	CheckFalse(CanRiding);

	float targetArmLength = SpringArm->TargetArmLength;
	targetArmLength += 50.0f;
	SpringArm->TargetArmLength = UKismetMathLibrary::Clamp(targetArmLength, 500, 1000);
	float cameraHeight = Camera->GetRelativeLocation().Z;
	cameraHeight += 10.0f;
	Camera->SetRelativeLocation(FVector(0.0f, 0.0f, UKismetMathLibrary::Clamp(cameraHeight, 140, 240)));
}

void ACHorse::OnRun()
{
	CheckFalse(CanRiding);

	GetCharacterMovement()->MaxWalkSpeed = Status->GetHorse_SprintSpeed();
}

void ACHorse::OffRun()
{
	CheckFalse(CanRiding);

	GetCharacterMovement()->MaxWalkSpeed = Status->GetHorse_WalkSpeed();
}

void ACHorse::OffRiding()
{
	CheckTrue(Rider == NULL);

	UCRidingComponent* riding = CHelpers::GetComponent<UCRidingComponent>(Rider);
	CheckNull(riding);

	riding->OffRiding();

	Rider = NULL;
}

void ACHorse::SetCanRiding(bool InCanRiding)
{
	CanRiding = InCanRiding;
}

void ACHorse::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(CanRiding);

	if (Cast<ACharacter>(OtherActor) == this)
	{
		return;
	}

	if (Cast<ACharacter>(OtherActor) == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		ACharacter* rider = Cast<ACharacter>(OtherActor);
		CheckNull(rider);
		
		Rider = rider;

		UCRidingComponent* riding = CHelpers::GetComponent<UCRidingComponent>(rider);
		CheckNull(riding);

		UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(rider);
		CheckNull(state);

		//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		if (state->IsRiding_Movement() == true)
		{
			return;
		}

		riding->SetOnRiding();

		// Mount 위치 Setting
		FVector mountLocation = GetMesh()->GetSocketLocation("MountPoint"); // Socket 위치 가져오기
		riding->SetMountValue(mountLocation, GetActorRotation());
		riding->SetMountDirectionVec(GetActorRightVector(), GetActorRightVector() * -1.0f);
		riding->SetHorse(this);

		// 외적을 통해서 좌우 확인 -> Mount 왼쪽 오른쪽 Setting
		// 1. 타겟과 나의 방향 구하기
		FVector playerLocation = rider->GetActorLocation();
		FVector horseLocation = GetActorLocation();
		FVector direction = playerLocation - horseLocation; // 말에서 플레이어를 바라보는 방향
		// 2. 말의 정면벡터와 외적
		FVector crossVec =  UKismetMathLibrary::Cross_VectorVector(direction, GetActorForwardVector());
		// 3. 외적값과 Up벡터를 내적
		float dotValue = UKismetMathLibrary::Dot_VectorVector(crossVec, GetActorUpVector());

		if (dotValue > 0)
		{
			//CLog::Log("SetMountLeft");
			riding->SetMountLeft();
		}
		else if (dotValue < 0)
		{
			//CLog::Log("SetMountRight");
			riding->SetMountRight();
		}
		else
		{
			// 말과 같은 선상에 위치
			//CLog::Log("dotValue == 0 ");
			riding->SetOffRiding();
		}
		
		if (Cast<ACPlayer>(Rider)->GetStatusBar() != NULL)
		{
			Cast<ACPlayer>(Rider)->GetStatusBar()->OnRiding();
		}
	}
}

void ACHorse::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<ACharacter>(OtherActor) == this)
	{
		return;
	}

	if (Cast<ACharacter>(OtherActor) == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		ACharacter* rider = Cast<ACharacter>(OtherActor);
		CheckNull(rider);

		UCRidingComponent* riding = CHelpers::GetComponent<UCRidingComponent>(rider);
		CheckNull(riding);
		
		UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(rider);
		CheckNull(state);
		
		if (state->IsRiding_Movement() == true)
		{
			return;
		}

		riding->SetOffRiding();

		if (Cast<ACPlayer>(Rider)->GetStatusBar() != NULL)
		{
			Cast<ACPlayer>(Rider)->GetStatusBar()->OffRiding();
		}

		Rider = NULL;
	}
}


