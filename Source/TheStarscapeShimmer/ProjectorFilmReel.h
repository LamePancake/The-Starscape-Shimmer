// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ProjectorFilmReel.generated.h"

UCLASS()
class THESTARSCAPESHIMMER_API AProjectorFilmReel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectorFilmReel();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Runs or stops the reel
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Reel)
	void RunReel();

};
