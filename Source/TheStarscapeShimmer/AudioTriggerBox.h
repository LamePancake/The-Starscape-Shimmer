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

	// Starts the shimmer's voice
	void Speak();

	// The list of clips to play
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		TArray<USoundBase*> ShimmerVoiceClips;

	// The delays between each clip (in seconds)
	// Note that you must have at least two clips for this to take effect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		TArray<float> ShimmerVoiceDelays;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		AEmitter* ShimmerVoiceParticles;

private:
	float TimeSinceLastClip;
	AAmbientSound* Speaker;
	bool HasSpoken;
	
};
