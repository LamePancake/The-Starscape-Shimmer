// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "ObjectiveManager.h"


// Sets default values
AObjectiveManager::AObjectiveManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AObjectiveManager::BeginPlay()
{
	Super::BeginPlay();

	// TODO: Take this out once we finish the game
	if (Objectives.Num() == 0) {
		UE_LOG(LogTemp, Warning, TEXT("ObjectiveManager.BeginPlay: No objectives!"));
		return;
	}

	Objectives[0]->OnActivate();
}

int32 AObjectiveManager::NumRemainingObjectives() const {
	return Objectives.Num() - CurrentObjectiveIdx;
}

int32 AObjectiveManager::NumCompletedObjectives() const {
	return CurrentObjectiveIdx;
}

int32 AObjectiveManager::NumObjectives() const {
	return Objectives.Num();
}

// Called every frame
void AObjectiveManager::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	if (CurrentObjectiveIdx == Objectives.Num()) {
		return;
	}

	bool Result;
	Objectives[CurrentObjectiveIdx]->IsComplete(Result);
	if (Result) {
		Objectives[CurrentObjectiveIdx]->OnComplete();
		CurrentObjectiveIdx++;
		if (CurrentObjectiveIdx == Objectives.Num()) {
			UE_LOG(LogTemp, Warning, TEXT("Finished all objectives"));
			return;
		}
		Objectives[CurrentObjectiveIdx]->OnActivate();
	}
}

