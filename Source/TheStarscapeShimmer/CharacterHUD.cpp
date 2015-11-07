// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "CharacterHUD.h"

ACharacterHUD::ACharacterHUD()
{
	//Can set to false if we dont not want to draw the hud for some reason
	DrawTheHUD = true;
	DrawReticle = true;
	DrawNoteString = false;
	DrawSafeString = false;
	SafeString = "";

	DefaultFontScale = 1.5f;
}


void ACharacterHUD::DrawHUD_Reticle()
{
	//Get the width and height of the screen
	const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());

	//Calculate the center of the screen            
	const FVector2D  ViewportCenter = FVector2D(ViewportSize.X / 2, ViewportSize.Y / 2);

	//draw the reticle
	DrawFullSizeTile(reticle, ViewportCenter.X - (reticle->GetSurfaceWidth() / 2), ViewportCenter.Y - (reticle->GetSurfaceHeight() / 2), FColor(255, 255, 255, 255));
}

void ACharacterHUD::DrawHUD_String(FString string)
{
	//Get the width and height of the screen
	const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());

	//Calculate the center of the screen            
	const FVector2D  ViewportCenter = FVector2D(ViewportSize.X / 2, ViewportSize.Y / 2);

	DrawText(VerdanaFont, string, ViewportCenter.X + ((string.Len() + 8) / 2), ViewportSize.Y * 0.55, FColor(255, 255, 255, 255), DefaultFontScale);
}

void ACharacterHUD::DrawHUD()
{
	//Draw HUD?
	if (!DrawTheHUD) 
		return;

	Super::DrawHUD();

	if (!Canvas) 
		return;

	// Draws the reticle.
	if (DrawReticle)
		DrawHUD_Reticle();

	if (DrawNoteString)
		DrawHUD_String(NoteString);

	if (DrawSafeString)
		DrawHUD_String(SafeString);

	//Put other hud related things here.
}