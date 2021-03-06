// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "NotePickup.h"
#include "TheStarscapeShimmerGameMode.h"
#include "CharacterHUD.h"

ANotePickup::ANotePickup()
{
	NoteIndex = 0;
	Fade = 0.0f;
}

const FString ANotePickup::Notes[] =
{
	"This is a test note\nThis is a test note.\nThis is a test note\nThis is a test note.\nThis is a test note\nThis is a test note.\nThis is a test note\nThis is a test note.\n",
	"This is another Test\nThis is another test.",
	"Dear group,\nIf you are reading this then my code got commited.\nI am sorry to tell you that this note is just a test note.\n\nWhy did you spend time reading this?\n\nSincerely,\nTrevor",
	"RUN!"
};

void ANotePickup::Read()
{
	APlayerController* c = GetWorld()->GetFirstPlayerController();
	if (c)
	{
		ACharacterHUD* h = Cast<ACharacterHUD>(c->GetHUD());
		if (h)
		{
			h->DrawNoteString = !h->DrawNoteString;
			h->DrawSafeString = false;
			h->NoteString = Notes[NoteIndex];
			h->NoteImage = NoteImage;
			h->DrawNoteImage = !h->DrawNoteImage;

			if (h->DrawNoteString)
				Fade = 1.0;
			else
				Fade = -1.0;
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
			h->DrawNoteString = false;
			h->DrawNoteImage = false;
			h->NoteString = "";
			Fade = -1.0;
		}
	}
}

void ANotePickup::Tick(float DeltaTime)
{
	if (Fade != 0)
	{
		APlayerController* c = GetWorld()->GetFirstPlayerController();
		if (c)
		{
			ACharacterHUD* h = Cast<ACharacterHUD>(c->GetHUD());
			if (h)
			{
				h->BlackBackgroundAlpha = FMath::Clamp(h->BlackBackgroundAlpha + (200 * DeltaTime * Fade), 0.0f, 150.0f);

				if (h->BlackBackgroundAlpha == 0.0f || h->BlackBackgroundAlpha == 150)
					Fade = 0;
			}
		}
	}
}


