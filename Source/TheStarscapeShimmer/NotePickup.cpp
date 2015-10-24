// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "NotePickup.h"
#include "TheStarscapeShimmerGameMode.h"
#include "CharacterHUD.h"

void ANotePickup::Read()
{
	APlayerController* c = GetWorld()->GetFirstPlayerController();
	if (c)
	{
		ACharacterHUD* h = Cast<ACharacterHUD>(c->GetHUD());
		if (h)
		{
			h->DrawReticle = !h->DrawReticle;
			h->DrawString = !h->DrawString;
			h->NoteString = Note;
		}
	}
}

void ANotePickup::OnDrop_Implementation()
{
	Super::OnDrop_Implementation();
	APlayerController* c = GetWorld()->GetFirstPlayerController();
	if (c)
	{
		ACharacterHUD* h = Cast<ACharacterHUD>(c->GetHUD());
		if (h)
		{
			h->DrawReticle = true;
			h->DrawString = false;
			h->NoteString = "";
		}
	}
}


