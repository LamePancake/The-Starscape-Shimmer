// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "SafeInteract.h"
#include "CharacterHUD.h"

// Sets default values
ASafeInteract::ASafeInteract()
{
	Code = "19901403";

	// The safe is locked.
	IsLocked = true;

	EnteringCombination = false;
	TestCombination = "";
	CombinationLength = 0;
}

//Overrides the interation method
void ASafeInteract::OnInteraction_Implementation(AFirstPersonCharacter* Character)
{
	Super::OnInteraction_Implementation(Character);

	if (IsLocked)
	{
		CharacterReference = Character;
		EnteringCombination = !EnteringCombination;
	}

	UAudioComponent* SpeakerAudio = SafeSpeaker->GetAudioComponent();
	UAudioComponent* SpeakerAudio2 = SafeUnlockSpeaker->GetAudioComponent();

	if (!IsLocked)
	{
		SpeakerAudio2->Play();
		UnlockSafe();
		return;
	}
}

bool ASafeInteract::EnterCombination()
{
	if (TestCombination.Compare(Code) == 0)
	{
		return true;
	}

	return false;
}

void ASafeInteract::Tick(float DeltaTime)
{
	APlayerController* c = GetWorld()->GetFirstPlayerController();
	if (!c)
		return;

	ACharacterHUD* h = Cast<ACharacterHUD>(c->GetHUD());
	if (!h)
		return;

	if (EnteringCombination)
	{
		h->DrawSafeString = true;
		FString string = TestCombination;
		
		if (CombinationLength < 8)
		{
			string += "_";
			for (int i = 0; i <  (8 - CombinationLength) - 1; i++)
			{
				string += " ";
			}
		}
		h->SafeString = string;

		APlayerController* PlayerController = Cast<APlayerController>(CharacterReference->GetController());
		if (PlayerController)
		{
			if (PlayerController->WasInputKeyJustReleased(EKeys::One) || PlayerController->WasInputKeyJustReleased(EKeys::NumPadOne))
			{
				if (CombinationLength < 8)
				{
					TestCombination.Append("1");
					CombinationLength++;
				}
			}
			else if (PlayerController->WasInputKeyJustReleased(EKeys::Two) || PlayerController->WasInputKeyJustReleased(EKeys::NumPadTwo))
			{
				if (CombinationLength < 8)
				{
					TestCombination.Append("2");
					CombinationLength++;
				}
			}
			else if (PlayerController->WasInputKeyJustReleased(EKeys::Three) || PlayerController->WasInputKeyJustReleased(EKeys::NumPadThree))
			{
				if (CombinationLength < 8)
				{
					TestCombination.Append("3");
					CombinationLength++;
				}
			}
			else if (PlayerController->WasInputKeyJustReleased(EKeys::Four) || PlayerController->WasInputKeyJustReleased(EKeys::NumPadFour))
			{
				if (CombinationLength < 8)
				{
					TestCombination.Append("4");
					CombinationLength++;
				}
			}
			else if (PlayerController->WasInputKeyJustReleased(EKeys::Five) || PlayerController->WasInputKeyJustReleased(EKeys::NumPadFive))
			{
				if (CombinationLength < 8)
				{
					TestCombination.Append("5");
					CombinationLength++;
				}
			}
			else if (PlayerController->WasInputKeyJustReleased(EKeys::Six) || PlayerController->WasInputKeyJustReleased(EKeys::NumPadSix))
			{
				if (CombinationLength < 8)
				{
					TestCombination.Append("6");
					CombinationLength++;
				}
			}
			else if (PlayerController->WasInputKeyJustReleased(EKeys::Seven) || PlayerController->WasInputKeyJustReleased(EKeys::NumPadSeven))
			{
				if (CombinationLength < 8)
				{
					TestCombination.Append("7");
					CombinationLength++;
				}
			}
			else if (PlayerController->WasInputKeyJustReleased(EKeys::Eight) || PlayerController->WasInputKeyJustReleased(EKeys::NumPadEight))
			{
				if (CombinationLength < 8)
				{
					TestCombination.Append("8");
					CombinationLength++;
				}
			}
			else if (PlayerController->WasInputKeyJustReleased(EKeys::Nine) || PlayerController->WasInputKeyJustReleased(EKeys::NumPadNine))
			{
				if (CombinationLength < 8)
				{
					TestCombination.Append("9");
					CombinationLength++;
				}
			}
			else if (PlayerController->WasInputKeyJustReleased(EKeys::Zero) || PlayerController->WasInputKeyJustReleased(EKeys::NumPadZero))
			{
				if (CombinationLength < 8)
				{
					TestCombination.Append("0");
					CombinationLength++;
				}
			}
			else if (PlayerController->WasInputKeyJustReleased(EKeys::BackSpace))
			{
				if (CombinationLength > 0)
				{
					TestCombination.RemoveAt(TestCombination.Len() - 1, 1, true);
					CombinationLength--;
				}
			}

			if (CombinationLength == 8)
			{
				UAudioComponent* SpeakerAudio = SafeSpeaker->GetAudioComponent();
				UAudioComponent* SpeakerAudio2 = SafeUnlockSpeaker->GetAudioComponent();

				if (PlayerController->WasInputKeyJustReleased(EKeys::Enter))
				{
					if (EnterCombination())
					{
						EnteringCombination = false;
						h->DrawSafeString = false;
						h->SafeString = "";

						SpeakerAudio2->Play();
						IsLocked = false;
						UnlockSafe();
					}
					else
					{
						if (!SpeakerAudio->IsPlaying())
							SpeakerAudio->Play();
					}
				}
			}
		}
	}
	else
	{
		h->DrawSafeString = false;
		h->SafeString = "";
	}
}


