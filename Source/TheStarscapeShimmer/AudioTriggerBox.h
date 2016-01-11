// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/TriggerBox.h"
#include "AudioTriggerBox.generated.h"

/**
 * 
 */
UCLASS()
class THESTARSCAPESHIMMER_API AAudioTriggerBox : public ATriggerBox
{
	GENERATED_BODY()

public:
	AAudioTriggerBox();

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	///<summary>Determines whether audio is currently playing.</summary>
	///<returns>Whether the AudioTriggerBox is current playing audio.</returns>
	bool IsPlaying();

	///<summary>Resets the AudioTriggerBox so that it will play audio again the next time the player overlaps with it.</summary>
	///<remarks>Note that this immediately stops any playing audio and resets to the first clip. Poll IsPlaying() and
	/// call this method when it returns false to stop only after all audio has finished.</remarks>
	void Reset();

	UFUNCTION()
	void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	///<summary>Starts playing the audio clips.</summary>
	void Play();

	///<summary>The list of clips to play.</summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		TArray<USoundBase*> SoundClips;

	///<summary>The delays between each clip (in seconds).</summary>
	///<remarks>Note that you must have at least two clips for this to take effect.</remarks>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		TArray<float> SoundClipDelays;

private:
	float TimeToNextClip;
	int32 CurrentClipIdx;
	bool bPlaying;
	AAmbientSound* Speaker;
	bool bPlayedAll;
	
};
