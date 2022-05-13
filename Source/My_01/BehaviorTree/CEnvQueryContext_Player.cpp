#include "BehaviorTree/CEnvQueryContext_Player.h"
#include "Global.h"

#include "Characters/CEnemy_AI.h"
#include "Characters/CPlayer.h"

#include "Components/CBehaviorCompoent.h"

#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

void UCEnvQueryContext_Player::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(QueryInstance.Owner.Get());
	CheckNull(ai);

	UCBehaviorCompoent* behavior = CHelpers::GetComponent<UCBehaviorCompoent>(ai->GetController());
	CheckNull(behavior);

	if (behavior->GetTargetPlayer() != NULL)
	{
		UEnvQueryItemType_Actor::SetContextHelper(ContextData, behavior->GetTargetPlayer());
	}

}