// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Objective.generated.h"

UCLASS()
class THESTARSCAPESHIMMER_API AObjective : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjective();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Objective)
		void OnActivate();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Objective)
		void OnComplete();

	UFUNCTION(BlueprintImplementableEvent, Category = Objective)
		void IsComplete(bool& Result);
};
