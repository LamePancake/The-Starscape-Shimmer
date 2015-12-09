// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "FirstPersonCharacter.h"
#include "AudioTriggerBox.h"

AAudioTriggerBox::AAudioTriggerBox() {
	//Speaker = GetWorld()->SpawnActor<AAmbientSound>(AAmbientSound::StaticClass());
}

void AAudioTriggerBox::BeginPlay() {
	Speaker->SetActorLocation(GetActorLocation());
}

bool AAudioTriggerBox::IsPlaying() {
	return IsSpeaking;
}

void AAudioTriggerBox::Reset() {
	if (IsSpeaking) return;
	HasSpoken = false;
	TimeToNextClip = 0.f;
	CurrentClipIdx = -1;
}

void AAudioTriggerBox::Tick(float DeltaTime) {
	if (IsSpeaking) {
		TimeToNextClip -= fmaxf(0, DeltaTime);
		if (TimeToNextClip == 0) {
			CurrentClipIdx++;

			// If we've gone through all of the clips, then reset all the properties and get out of here
			if (CurrentClipIdx == ShimmerVoiceClips.Num()) {
				HasSpoken = true;
				IsSpeaking = false;
				for (TActorIterator<AAmbientSound> ActorItr(GetWorld()); ActorItr; ++ActorItr)
				{
					if (*ActorItr == Speaker) continue;
					ActorItr->GetAudioComponent()->SetVolumeMultiplier(1.f);
				}
				AFirstPersonCharacter* Player = Cast<AFirstPersonCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
				Player->MovementSpeedMultiplier = 1.f;
				return;
			}

			// Otherwise, we have some work to do
			// Only add on the delay if we're not on the last clip
			TimeToNextClip = ShimmerVoiceClips[CurrentClipIdx]->GetDuration();
			TimeToNextClip += CurrentClipIdx == ShimmerVoiceClips.Num() - 1 ? 0 : ShimmerVoiceDelays[CurrentClipIdx];
			Speaker->GetAudioComponent()->SetSound(ShimmerVoiceClips[CurrentClipIdx]);
			Speaker->GetAudioComponent()->Play();

		}
	}
}

void AAudioTriggerBox::Play() {
	if (HasSpoken) return;
	
	Reset();

	AFirstPersonCharacter* Player = Cast<AFirstPersonCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	Player->MovementSpeedMultiplier = 0.1f;
	
	// EngineUtils.h
	for (TActorIterator<AAmbientSound> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (*ActorItr == Speaker) continue;
		ActorItr->GetAudioComponent()->SetVolumeMultiplier(0.1f);
	}
}
