#include "Characters/CAIController.h"
#include "Global.h"

#include "CEnemy_AI.h"
#include "CPlayer.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

#include "Components/CBehaviorCompoent.h"

ACAIController::ACAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateActorComponent<UBlackboardComponent>(this, &Blackboard, "Blackboard");
	CHelpers::CreateActorComponent<UAIPerceptionComponent>(this, &Perception, "Perception");
	CHelpers::CreateActorComponent<UCBehaviorCompoent>(this, &Behavior, "Behavior");

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	Sight->SightRadius = 2000.0f;
	Sight->LoseSightRadius = 2000.0f;
	Sight->PeripheralVisionAngleDegrees = 120.0f;
	Sight->SetMaxAge(2.0f);

	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;
	Sight->DetectionByAffiliation.bDetectNeutrals = false;

	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());
}

void ACAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bDrawDebug == true)
	{
		FVector center = OwnerEnemy->GetActorLocation();
		center.Z -= AdjustCircleHeight;

		DrawDebugCircle(GetWorld(), center, Sight->SightRadius, 300, FColor::Green, false, -0.1f, 0, 0, FVector::RightVector, FVector::ForwardVector);
		//DrawDebugCircle(GetWorld(), center, BehaviorRange, 300, FColor::Red, false, -0.1f, 0, 0, FVector::RightVector, FVector::ForwardVector);
	}
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	OwnerEnemy = Cast<ACEnemy_AI>(InPawn);
	SetGenericTeamId(OwnerEnemy->GetTeamID());

	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdated);

	UseBlackboard(OwnerEnemy->GetBehaviorTree()->BlackboardAsset, Blackboard);
	Behavior->SetBlackboard(Blackboard);

	RunBehaviorTree(OwnerEnemy->GetBehaviorTree());
}

void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();

	Perception->OnPerceptionUpdated.Clear();
}

float ACAIController::GetSightRadius()
{
	return Sight->SightRadius;
}

void ACAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdataedActors)
{
	TArray<AActor*> actors;
	Perception->GetCurrentlyPerceivedActors(NULL, actors);

	ACPlayer* player = NULL;
	for (AActor* actor : actors)
	{
		player = Cast<ACPlayer>(actor);

		if (player != NULL)
		{
			break;
		}
	}

	Blackboard->SetValueAsObject("Player", player);
}

