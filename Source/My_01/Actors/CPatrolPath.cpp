#include "Actors/CPatrolPath.h"
#include "Global.h"

#include "Components/TextRenderComponent.h"
#include "Components/SplineComponent.h"

ACPatrolPath::ACPatrolPath()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene");
	CHelpers::CreateComponent<USplineComponent>(this, &Spline, "Spline", Scene);
	CHelpers::CreateComponent<UTextRenderComponent>(this, &Text, "Text", Scene);

	Text->SetRelativeLocation(FVector(0, 0, 120));
	Text->SetRelativeRotation(FRotator(0, 180, 0));
	Text->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	Text->TextRenderColor = FColor::Red;
}

void ACPatrolPath::OnConstruction(const FTransform& Transform)
{
	//Text->Text = FText::FromString(GetActorLabel());
	if (Spline != NULL)
	{
		Spline->SetClosedLoop(bLoop);
	}
}

void ACPatrolPath::BeginPlay()
{
	Super::BeginPlay();
	if (Text != NULL)
	{
		Text->SetVisibility(false);
	}
}
