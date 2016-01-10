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
	BlackBackgroundAlpha = 0;
	SafeString = "";

	FadeInAtStart = false;
	FadeAtPortal = false;
	FadeInAtEnd = false;
	PortalTeleport = false;
	FadeAfterPortal = false;
	FadeAtGameOver = false;

	DrawNoteImage = false;

	PauseFade = false;
	PauseFadeOut = false;

	DisplayEnding = false;

	DefaultFontScale = 1.5f;
	EndingCounter = 0;

	EndingString1 = "The next morning, the police came to investigate the Starscape,";
	EndingString2 = "only to find a film left playing. They found no traces of life.";
	EndingString3 = "It was as if you never existed.";
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

void ACharacterHUD::DrawHUD_String(UFont* font, FString string, const float& X, const float& Y, const float& TheScale)
{
	DrawText(font, string, X, Y, FColor(255, 255, 255, 255), TheScale);
}

void ACharacterHUD::DrawHUD_Image(UTexture2D* tex, float alpha, float x, float y, float width, float height)
{
	DrawImage(tex, x, y, FColor(255, 255, 255, alpha), width, height);
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

	if (FadeInAtStart)
	{
		BlackBackgroundAlpha -= 3;

		if (BlackBackgroundAlpha <= 0)
		{
			FadeInAtStart = false;
			BlackBackgroundAlpha = 0;
		}
	}

	if (FadeAtPortal)
	{
		BlackBackgroundAlpha += 6;

		if (BlackBackgroundAlpha >= 255)
		{
			BlackBackgroundAlpha = 255;
			PortalTeleport = true;
			FadeAtPortal = false;
			FadeAfterPortal = true;
		}
	}

	if (FadeAfterPortal)
	{
		BlackBackgroundAlpha -= 6;

		if (BlackBackgroundAlpha <= 0)
		{
			FadeAfterPortal = false;
			BlackBackgroundAlpha = 0;
		}
	}

	if (FadeAtGameOver) {
		BlackBackgroundAlpha += 2;

		if (BlackBackgroundAlpha >= 255)
		{
			BlackBackgroundAlpha = 255;
			DisplayEnding = true;
		}
	}

	if (PauseFade)
	{
		BlackBackgroundAlpha += 8;
		if (BlackBackgroundAlpha >= 150)
		{
			BlackBackgroundAlpha = 150;
			PauseFade = false;
		}
	}

	if (PauseFadeOut)
	{
		BlackBackgroundAlpha -= 8;
		if (BlackBackgroundAlpha <= 0)
		{
			BlackBackgroundAlpha = 0;
			PauseFadeOut = false;
		}
	}

	if (BlackBackgroundAlpha != 0)
	{
		//Get the width and height of the screen
		const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
		DrawHUD_Image(BlackBackground, BlackBackgroundAlpha, 0, 0, ViewportSize.X, ViewportSize.Y);

		if (BlackBackgroundAlpha == 150.0f)
		{
			if (DrawNoteString)
			{
				if (DrawNoteImage)
				{
					float Height = ViewportSize.Y;
					float Width = NoteImage->GetSizeX() * ViewportSize.Y / NoteImage->GetSizeY();
					float XDrawPos = (ViewportSize.X - Width) / 2;
					const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
					DrawHUD_Image(NoteImage, 255, XDrawPos, 0, Width, Height);
				}
				else
				{
					//Get the width and height of the screen
					const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());

					DrawHUD_String(VerdanaFont, NoteString, ViewportSize.X * 0.2, ViewportSize.Y * 0.3, DefaultFontScale);
				}
			}

			if (DrawSafeString)
			{
				//Get the width and height of the screen
				const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
				//Calculate the center of the screen            
				const FVector2D  ViewportCenter = FVector2D(ViewportSize.X / 2, ViewportSize.Y / 2);

				DrawHUD_String(VerdanaFont, "Enter the Combination:", ViewportCenter.X - (VerdanaFont->GetStringSize(TEXT("Enter the Combination:")) * 2.5 / 2), ViewportSize.Y * 0.35, 2.5f);
				DrawHUD_String(VerdanaFont, SafeString, ViewportCenter.X - (VerdanaFont->GetStringSize(*(SafeString)) * 2 / 2), ViewportSize.Y * 0.6, 2.0f);
			}
		}

		if (DisplayEnding)
		{
			EndingCounter++;
			//Get the width and height of the screen
			const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
			//Calculate the center of the screen            
			const FVector2D  ViewportCenter = FVector2D(ViewportSize.X / 2, ViewportSize.Y / 2);

			DrawHUD_String(VerdanaFont, EndingString1, ViewportCenter.X - (VerdanaFont->GetStringSize(EndingString1.GetCharArray().GetData()) * 1.5 / 2), ViewportSize.Y * 0.35, 1.5f);
			DrawHUD_String(VerdanaFont, EndingString2, ViewportCenter.X - (VerdanaFont->GetStringSize(EndingString2.GetCharArray().GetData()) * 1.5 / 2), ViewportSize.Y * 0.5, 1.5f);
			DrawHUD_String(VerdanaFont, EndingString3, ViewportCenter.X - (VerdanaFont->GetStringSize(EndingString3.GetCharArray().GetData()) * 1.5 / 2), ViewportSize.Y * 0.65, 1.5f);

			if (EndingCounter > 1000)
			{
				FGenericPlatformMisc::RequestExit(false);
			}
		}
	}





	//Put other hud related things here.
}