// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "CharacterHUD.h"

ACharacterHUD::ACharacterHUD()
{
	//Can set to false if we dont not want to draw the hud for some reason
	DrawTheHUD = true;
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

void ACharacterHUD::DrawHUD()
{
	//Draw HUD?
	if (!DrawTheHUD) 
		return;

	Super::DrawHUD();

	if (!Canvas) 
		return;

	// Draws the reticle.
	DrawHUD_Reticle();

	//Put other hud related things here.
}