// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "FirstPersonCharacter.h"
#include "AudioTriggerBox.h"

AAudioTriggerBox::AAudioTriggerBox() {
	PrimaryActorTick.bCanEverTick = true;
}

void AAudioTriggerBox::BeginPlay() {
	Speaker = GetWorld()->SpawnActor<AAmbientSound>(AAmbientSound::StaticClass());
	Speaker->SetActorLocation(GetActorLocation());
	GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &AAudioTriggerBox::OnOverlapBegin);
}

bool AAudioTriggerBox::IsPlaying() {
	return bPlaying;
}

void AAudioTriggerBox::Reset() {
	if (bPlaying) Speaker->GetAudioComponent()->Stop();
	bPlaying = false;
	bPlayedAll = false;
	TimeToNextClip = 0.f;
	CurrentClipIdx = -1;
}

void AAudioTriggerBox::Tick(float DeltaTime) {
	if (bPlaying) {

		TimeToNextClip -= DeltaTime;
		TimeToNextClip = fmaxf(0, TimeToNextClip);
		if (TimeToNextClip == 0) {
			CurrentClipIdx++;

			// If we've gone through all of the clips, then reset all the properties and get out of here
			if (CurrentClipIdx == SoundClips.Num()) {
				bPlayedAll = true;
				bPlaying = false;
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
			TimeToNextClip = SoundClips[CurrentClipIdx]->GetDuration();
			TimeToNextClip += CurrentClipIdx == SoundClips.Num() - 1 ? 0 : SoundClipDelays[CurrentClipIdx];
			Speaker->GetAudioComponent()->SetSound(SoundClips[CurrentClipIdx]);
			Speaker->GetAudioComponent()->Play();

		}
	}
}

void AAudioTriggerBox::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Dirty hack to determine whether we collided with the player
	UCapsuleComponent* PlayerComponent = Cast<UCapsuleComponent>(OtherComp);
	// Other Actor is the actor that triggered the event. Check that is not ourself.  
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && PlayerComponent && !bPlayedAll)
	{
		Play();
	}
}

void AAudioTriggerBox::Play() {
	if (bPlayedAll) return;
	
	Reset();

	AFirstPersonCharacter* Player = Cast<AFirstPersonCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	Player->MovementSpeedMultiplier = 0.1f;
	
	// EngineUtils.h
	for (TActorIterator<AAmbientSound> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (*ActorItr == Speaker) continue;
		ActorItr->GetAudioComponent()->SetVolumeMultiplier(0.1f);
	}
	bPlaying = true;
}
