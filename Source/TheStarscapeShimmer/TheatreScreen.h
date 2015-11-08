// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractableObject.h"
#include "TheatreScreen.generated.h"

/**
 * 
 */
UCLASS()
class THESTARSCAPESHIMMER_API ATheatreScreen : public AInteractableObject
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Materials)
	class UMaterial* ScreenOffMaterial;
	
};
