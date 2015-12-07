// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ObjectiveManager.h"
#include "ShimmerSpawner.generated.h"

/**
 * Spawns shimmers around the player with the given properties.
 * As more objectives are completed, more shimmers will spawn. Once all objectives (or objectives up to the specified index) are complete, "FinalRate" will be the spawn rate.
 */
UCLASS()
class THESTARSCAPESHIMMER_API AShimmerSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShimmerSpawner();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// The final rate (number spawned per second) at which shimmers should be spawned
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShimmerProperties)
		float FinalRate;

	// The initial rate (number spawned per second) at which shimmers should be spawned
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShimmerProperties)
		float StartRate;

	// The starting distance from the player at which shimmers can spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShimmerProperties)
		float StartInnerRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShimmerProperties)
		float StartOuterRadius;

	// The final distance from the player at which shimmers can spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShimmerProperties)
		float FinalInnerRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShimmerProperties)
		float FinalOuterRadius;

	// The index that will be considered as the "first" index in the objective manager. This must be <= ObjectiveManager.NumObjectives() - 1.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShimmerProperties)
		int32 FirstIndex;

	// The index that will be considered as the "final" index in the objective manager. This must be <= ObjectiveManager.NumObjectives() - 1
	// If this is the same as FirstIndex, all final properties will be applied immediately.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShimmerProperties)
		int32 LastIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShimmerProperties)
		AObjectiveManager* ObjectiveManager;
};
