#include "Grapples/CGrappleTarget.h"
#include "Global.h"

#include "Components/BillboardComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"

#include "GameFramework/Character.h"

ACGrappleTarget::ACGrappleTarget()
{

	CHelpers::CreateComponent<UBillboardComponent>(this, &Billboard, "Billboard");
	RootComponent = Billboard;

	CHelpers::CreateComponent<UWidgetComponent>(this, &Widget, "Widget", Billboard);
	Widget->SetVisibility(false);

	CHelpers::CreateComponent<USphereComponent>(this, &Sphere, "Sphere", Billboard);

}

void ACGrappleTarget::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACGrappleTarget::SetActive(bool InActive)
{
	//FString debug = GetName() + " : Call SetActive";
	//CLog::Log(debug);
	Widget->SetVisibility(InActive);
}
