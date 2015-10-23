// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "ProjectorFilmReel.h"


// Sets default values
AProjectorFilmReel::AProjectorFilmReel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AProjectorFilmReel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectorFilmReel::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

