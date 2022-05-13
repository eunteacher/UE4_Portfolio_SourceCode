#include "Grapples/CGrappleRope.h"
#include "Global.h"

#include "C:\Program Files\Epic Games\UE_4.26\Engine\Plugins\Runtime\CableComponent\Source\CableComponent\Classes\CableComponent.h"

ACGrappleRope::ACGrappleRope()
{
	CableComponent->EndLocation = FVector(0.0f, 0.0f, 0.0f);
	CableComponent->CableWidth = 5.0f;
}
