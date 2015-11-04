// Fill out your copyright notice in the Description page of Project Settings.

//HUD based on this tutorial https://wiki.unrealengine.com/HUD,_Canvas,_Entire_Class_Sample,_Create_Buttons_%26_Draw_Materials
#pragma once
#pragma warning(disable: 4263)
#pragma warning(disable: 4264)

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

	/** Verdana */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	UFont* VerdanaFont;

	/** Font Scaling Used By Your HUD Code */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	float DefaultFontScale;

	// The reticle for the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = T2D)
	UTexture2D* reticle;
	
	// Just incase we dont want to draw the hud
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Options)
	bool DrawTheHUD;

	// True if we want to draw the reticle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Options)
	bool DrawReticle;

	// True if we want to draw a string
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Options)
	bool DrawString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Note)
	FString NoteString;

	// Add other hud things here like text or other things.

	//Draws the reticle on screen
	void DrawHUD_Reticle();

	void DrawHUD_String();


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

	//DrawText
	FORCEINLINE void DrawText(UFont* TheFont, const FString& TheStr, const float& X, const float& Y, const FLinearColor& TheColor, const float& TheScale, bool DrawOutline = false, const FLinearColor OutlineColor = FLinearColor(0, 0, 0, 1)) 
	{
		if (!Canvas)
		{
			return;
		}

		//Text and Font
		FCanvasTextItem NewText(FVector2D(X, Y), FText::FromString(TheStr), TheFont, TheColor);

		//Text Scale
		NewText.Scale.Set(TheScale, TheScale);

		//Outline gets its alpha from the main color
		NewText.bOutlined = true;
		NewText.OutlineColor = OutlineColor;
		NewText.OutlineColor.A = TheColor.A * 2;

		//Draw
		Canvas->DrawItem(NewText);
	}

	void DrawHUD();

};
