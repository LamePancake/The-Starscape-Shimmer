// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Objective.h"
#include "ObjectiveManager.generated.h"

UCLASS()
class THESTARSCAPESHIMMER_API AObjectiveManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjectiveManager();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category = ObjectiveInfo)
		int32 NumRemainingObjectives() const;

	UFUNCTION(BlueprintCallable, Category = ObjectiveInfo)
		int32 NumCompletedObjectives() const;

	UFUNCTION(BlueprintCallable, Category = ObjectiveInfo)
		int32 NumObjectives() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Objectives)
		TArray<AObjective*> Objectives;

private:
	uint32 CurrentObjectiveIdx;
};
