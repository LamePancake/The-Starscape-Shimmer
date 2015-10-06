// Fill out your copyright notice in the Description page of Project Settings.

//HUD based on this tutorial https://wiki.unrealengine.com/HUD,_Canvas,_Entire_Class_Sample,_Create_Buttons_%26_Draw_Materials
#pragma once

#include "GameFramework/HUD.h"
#include "CharacterHUD.generated.h"

/**
 * 
 */
UCLASS()
class THESTARSCAPESHIMMER_API ACharacterHUD : public AHUD
{
	GENERATED_BODY()

public:
	ACharacterHUD();

	// The reticle for the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = T2D)
	UTexture2D* reticle;
	
	// Just incase we dont want to draw the hud
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Options)
	bool DrawTheHUD;

	// Add other hud things here like text or other things.

	//Draws the reticle on screen
	void DrawHUD_Reticle();

	//Draws the texture on screen
	FORCEINLINE void DrawFullSizeTile(UTexture2D* tex, float x, float y, const FColor& Color)
	{
		if (!Canvas) 
			return;
		if (!tex) 
			return;

		Canvas->SetDrawColor(Color);

		//Draw
		Canvas->DrawTile(
			tex, x, y,
			tex->GetSurfaceWidth(), //screen width, Dont think these are the right comments but it works
			tex->GetSurfaceHeight(),  //screen height  Dont think these are the right comments but it works
			0, //texture start width
			0, //texture start height
			tex->GetSurfaceWidth(), //texture width from start
			tex->GetSurfaceHeight(), //texture height from start
			BLEND_Translucent
			);
	}

	void DrawHUD();

};
