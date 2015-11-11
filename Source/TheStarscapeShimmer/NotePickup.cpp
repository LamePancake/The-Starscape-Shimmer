// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "NotePickup.h"
#include "TheStarscapeShimmerGameMode.h"
#include "CharacterHUD.h"

ANotePickup::ANotePickup()
{
	NoteIndex = 0;
}

const FString ANotePickup::Notes[] =
{
	"This is a test note\nThis is a test note.\nThis is a test note\nThis is a test note.\nThis is a test note\nThis is a test note.\nThis is a test note\nThis is a test note.\n",
	"This is another Test\nThis is another test.",
	"Dear group,\nIf you are reading this then my code got commited.\nI am sorry to tell you that this note is just a test note.\n\nWhy did you spend time reading this?\n\nSincerely,\nTrevor"
};

void ANotePickup::Read()
{
	APlayerController* c = GetWorld()->GetFirstPlayerController();
	if (c)
	{
		c->SetIgnoreLookInput(!c->IsLookInputIgnored());
		c->SetIgnoreMoveInput(!c->IsMoveInputIgnored());

		ACharacterHUD* h = Cast<ACharacterHUD>(c->GetHUD());
		if (h)
		{
			h->DrawNoteString = !h->DrawNoteString;
			h->DrawSafeString = false;
			h->NoteString = Notes[NoteIndex];
		}
	}
}

void ANotePickup::OnDrop_Implementation()
{
	Super::OnDrop_Implementation();
	APlayerController* c = GetWorld()->GetFirstPlayerController();
	if (c)
	{
		c->SetIgnoreLookInput(false);
		c->SetIgnoreMoveInput(false);

		ACharacterHUD* h = Cast<ACharacterHUD>(c->GetHUD());
		if (h)
		{
			h->DrawNoteString = false;
			h->NoteString = "";
		}
	}
}


