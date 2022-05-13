#include "Actions/CAim.h"
#include "Global.h"
#include "CHUD.h"

#include "Camera/CameraComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Components/CStateComponent.h"

#include "Characters/CAnimInstance.h"

UCAim::UCAim()
{
	CHelpers::GetClass(&CrossHairClass, "WidgetBlueprint'/Game/Widgets/WB_CrossHair.WB_CrossHair_C'");
}

void UCAim::BeginPlay(ACharacter* InCharacter)
{
	OwnerCharacter = InCharacter;
	
	SpringArm = CHelpers::GetComponent<USpringArmComponent>(OwnerCharacter);
	Camera = CHelpers::GetComponent<UCameraComponent>(OwnerCharacter);
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);

	// ���� ����
	CrossHair = CreateWidget<UCUserWidget_CrossHair, APlayerController>(OwnerCharacter->GetController<APlayerController>(), CrossHairClass);
	CrossHair->AddToViewport();
	CrossHair->SetDisappear();

	AnimInstatnce = Cast<UCAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
}

void UCAim::OnAim()
{
	CheckNull(SpringArm);
	CheckNull(Camera);
	CheckNull(AnimInstatnce);
	CheckTrue(bInZoom);

	// OnAim�� �ʿ��� �� ����
	bInZoom = true;

	AnimInstatnce->SetAiming(bInZoom);
	// SpringArm
	SpringArm->bEnableCameraLag = false;
	SpringArm->TargetArmLength = 180.0f; // 300
	SpringArm->SocketOffset.Y = 45.0f; // 0
	// Camera
	Camera->FieldOfView = 45.0f; // 90
	Camera->SetRelativeLocation(FVector(0.0f, 0.0f, 20.0f));
	Camera->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

	CrossHair->SetAppear(); // ���� �ѱ�
}

void UCAim::OffAim()
{
	CheckNull(SpringArm);
	CheckNull(Camera);
	CheckNull(AnimInstatnce);
	CheckFalse(bInZoom);

	// OffAim�� �ʿ��� �� ����

	bInZoom = false;

	AnimInstatnce->SetAiming(bInZoom);

	// SpringArm
	SpringArm->bEnableCameraLag = true;
	SpringArm->TargetArmLength = 500.0f; // 180
	SpringArm->SocketOffset.Y = 0.0f; // 45
	// Camera
	Camera->FieldOfView = 90.0f; // 45
	Camera->SetRelativeLocation(FVector(0.0f, 0.0f, 140.0f));
	Camera->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));

	CrossHair->SetDisappear();
}

// ���� �����Ǹ�
// ������ ���� ���������� ����
void UCAim::ForwardTrace()
{
	CheckNull(SpringArm);
	CheckNull(Camera);
	CheckNull(CrossHair);
	CheckFalse(bInZoom);
	
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::None;
	TEnumAsByte<EObjectTypeQuery> ObjectType = EObjectTypeQuery::ObjectTypeQuery3;

	FVector start = Camera->GetComponentLocation();
	FVector end = start + (Camera->GetForwardVector() * 3000.0f);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwnerCharacter);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(ObjectType);
	FHitResult hitResult;

	bool result = UKismetSystemLibrary::LineTraceSingleForObjects
	(
		OwnerCharacter->GetWorld(),
		start,
		end,
		ObjectTypes,
		true,
		ActorsToIgnore,
		DrawDebugType,
		hitResult,
		true
	);

	if (result == true)
	{
		//CLog::Print("Trace True");
		CrossHair->OnFocus();
	}
	else
	{
		CrossHair->OffFocus();
	}

}
