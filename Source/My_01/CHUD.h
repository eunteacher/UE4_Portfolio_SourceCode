#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CHUD.generated.h"

UCLASS()
class MY_01_API ACHUD : public AHUD
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly)
		class UTexture2D* Texture;

public:
	FORCEINLINE void SetAppear() { bDraw = true; }
	FORCEINLINE void SetDisappear() { bDraw = true; }

public:
	ACHUD();
	void DrawHUD() override;

private:
	bool bDraw;
};
