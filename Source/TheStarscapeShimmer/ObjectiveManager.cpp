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
	Objectives[0]->OnActivate();
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

