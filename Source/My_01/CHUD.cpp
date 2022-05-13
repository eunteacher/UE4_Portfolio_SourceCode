#include "CHUD.h"
#include "Global.h"

#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"

ACHUD::ACHUD()
{
	CHelpers::GetAsset<UTexture2D>(&Texture, "Texture2D'/Game/Images/CrossHair/CrossHair.CrossHair'");
}

void ACHUD::DrawHUD()
{
	Super::DrawHUD();

	CheckFalse(bDraw);
	CheckNull(Texture);

	FVector2D center = FVector2D(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
	FVector2D size = FVector2D(Texture->GetSurfaceWidth() * 0.5f, Texture->GetSurfaceHeight() * 0.5f);
	FVector2D position = center - size;

	FCanvasTileItem tileItem(position, Texture->Resource, FLinearColor::White);
	tileItem.BlendMode = SE_BLEND_Translucent;

	Canvas->DrawItem(tileItem);
}