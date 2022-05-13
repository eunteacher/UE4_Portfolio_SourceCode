#include "CGameMode.h"
#include "Global.h"

ACGameMode::ACGameMode()
{
	CHelpers::GetClass<APawn>(&DefaultPawnClass, "Blueprint'/Game/Player/BP_CPlayer2.BP_CPlayer2_C'");
	//CHelpers::GetClass<AHUD>(&HUDClass, "Blueprint'/Game/BP_CHUD.BP_CHUD_C'");
}