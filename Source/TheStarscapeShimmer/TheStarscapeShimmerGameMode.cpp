// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "TheStarscapeShimmerGameMode.h"

ATheStarscapeShimmerGameMode::ATheStarscapeShimmerGameMode()
{
	// Load the hud blueprint and attach it to the gamemode
	static ConstructorHelpers::FClassFinder<AHUD> TheHUDOb(TEXT("/Game/Blueprints/BP_CharacterHUD.BP_CharacterHUD_C"));
	if (TheHUDOb.Class != NULL)
	{
		HUDClass = TheHUDOb.Class;
	}
}


