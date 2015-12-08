// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "ShimmerSpawner.h"
#include "Audio.h"
#include <cmath>

#define SHIMMER_POOL_SIZE (10)


// Sets default values
AShimmerSpawner::AShimmerSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	IsEnabled = false;
	TimeSinceLastSpawn = 0;
}

// Called when the game starts or when spawned
void AShimmerSpawner::BeginPlay()
{
	Super::BeginPlay();
	MinPitchMultiplier = MinPitchMultiplier < MIN_PITCH ? MIN_PITCH : MinPitchMultiplier;
	NumSteps = FinalObjective - StartObjective;
	// Initialise the object pools
	FActorSpawnParameters Params;
	Params.Template = ShimmerTemplate;
	for (int i = 0; i < SHIMMER_POOL_SIZE; i++) {
		AEmitter* Shimmer = GetWorld()->SpawnActor<AEmitter>(AEmitter::StaticClass(), Params);
		AAmbientSound* Sound = GetWorld()->SpawnActor<AAmbientSound>(AAmbientSound::StaticClass());
		
		// Set the particle emitter to not automatically activate
		Shimmer->Deactivate();
		Shimmer->GetParticleSystemComponent()->bAutoActivate = false;

		Sound->GetAudioComponent()->SetSound(ShimmerSound);

		ParticleInstances.Add(Shimmer);
		SoundInstances.Add(Sound);
		TimeLeft.Add(0);
	}
}

/**
 * Disables the shimmer spawner and stops all currently active shimmers and sounds. 
 */
void AShimmerSpawner::Disable()
{
	IsEnabled = false;
	for (int i = 0; i < ParticleInstances.Num(); i++) {
		if (TimeLeft[i] != 0) {
			ParticleInstances[i]->Deactivate();
			SoundInstances[i]->GetAudioComponent()->Stop();
			TimeLeft[i] = 0;
		}
	}
}

void AShimmerSpawner::Enable()
{
	IsEnabled = true;
}

// Called every frame
void AShimmerSpawner::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	
	// Clamp CurrentObjective in case it's bigger than the specified max
	int32 CurrentObjective = ObjectiveManager->CurrentObjectiveNum();
	CurrentObjective = CurrentObjective > FinalObjective ? FinalObjective : CurrentObjective;

	if (IsEnabled && CurrentObjective >= StartObjective)
	{
		// We'll grab the first available shimmer in case we have to spawn one later
		int AvailableShimmerIdx = -1;
		int AvailableSoundIdx = -1;

		// Check whether any of the current shimmers should be disabled
		for (int i = 0; i < ParticleInstances.Num(); i++) {
			if (TimeLeft[i] != 0)
			{
				TimeLeft[i] -= DeltaTime;
				if (TimeLeft[i] <= 0) {
					ParticleInstances[i]->Deactivate();
					SoundInstances[i]->GetAudioComponent()->Stop();
					TimeLeft[i] = 0;
				}
			}
			else
			{
				AvailableShimmerIdx = i;
				AvailableSoundIdx = i;
			}
		}

		TimeSinceLastSpawn += DeltaTime;

		// Linearly interpolate all the current values
		int32 NumStepsFromStart = CurrentObjective - StartObjective;
		double CurrentRate = StartRate + (((FinalRate - StartRate) / NumSteps) * (NumStepsFromStart));
		
		// Don't spawn any more shimmers until we've passed the specified amount of time
		if (TimeSinceLastSpawn < CurrentRate) return;

		double CurrentInnerRadius = StartInnerRadius + (((FinalInnerRadius - StartInnerRadius) / NumSteps) * (NumStepsFromStart));
		double CurrentOuterRadius = StartOuterRadius + (((FinalOuterRadius - StartOuterRadius) / NumSteps) * (NumStepsFromStart));
		double CurrentRadiusArea = CurrentOuterRadius - CurrentInnerRadius;
		FVector Centre = Character->GetActorLocation();

		// Determine random starting position within the specified space for the new shimmer
		double X = CurrentInnerRadius + ((RAND_MAX - rand()) / (double)RAND_MAX * CurrentRadiusArea);
		double Y = CurrentInnerRadius + ((RAND_MAX - rand()) / (double)RAND_MAX * CurrentRadiusArea);
		double Z = CurrentInnerRadius + ((RAND_MAX - rand()) / (double)RAND_MAX * CurrentRadiusArea);

		// Determine the direction (don't change Z since we want it to spawn at least above the player)
		X *= (rand() > RAND_MAX / 2) ? 1 : -1;
		Y *= (rand() > RAND_MAX / 2) ? 1 : -1;


		// We're spawning shimmers quicker than they're becoming available, so we'll make another one (and a sound)
		if (AvailableShimmerIdx == -1) {
			FActorSpawnParameters Params;
			Params.Template = ShimmerTemplate;
			AvailableShimmerIdx = ParticleInstances.Num();
			AEmitter* Shimmer = GetWorld()->SpawnActor<AEmitter>(AEmitter::StaticClass(), Params);
			AAmbientSound* Sound = GetWorld()->SpawnActor<AAmbientSound>(AAmbientSound::StaticClass());

			// Set the particle emitter to not automatically activate
			Shimmer->Deactivate();
			Shimmer->GetParticleSystemComponent()->bAutoActivate = false;

			Sound->GetAudioComponent()->SetSound(ShimmerSound);

			ParticleInstances.Add(Shimmer);
			SoundInstances.Add(Sound);
			TimeLeft.Add(0);
		}

		AEmitter* AvailableShimmer = ParticleInstances[AvailableShimmerIdx];
		AAmbientSound* AvailableSound = SoundInstances[AvailableShimmerIdx];

		FVector SpawnLocation(X + Centre.X, Y + Centre.Y, Z + Centre.Z);
		AvailableShimmer->SetActorLocation(SpawnLocation);
		AvailableSound->SetActorLocation(SpawnLocation);
		// TODO: Change volume and pitch based on linear interpolation values
		float Distance = sqrtf((X * X) + (Y * Y) + (Z * Z));
		float NewPitchMultiplier = fmaxf(MinPitchMultiplier, 1 - Distance * PitchLowerRate);
		
		// Start the shimmer
		AvailableShimmer->Activate();
		AvailableShimmer->SetActorHiddenInGame(false);
		AvailableSound->GetAudioComponent()->SetPitchMultiplier(NewPitchMultiplier);
		TimeLeft[AvailableShimmerIdx] = ShimmerSound->Duration * (1 / NewPitchMultiplier); // Adjust the sound duration so that it plays fully
		AvailableSound->GetAudioComponent()->Play();
		TimeSinceLastSpawn = 0;
	}
}

