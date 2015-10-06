// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Materials/Material.h>
#include "InteractableObject.h"
#include "ProjectorInteract.generated.h"

/**
 * 
 */
UCLASS()
class THESTARSCAPESHIMMER_API AProjectorInteract : public AInteractableObject
{
	GENERATED_BODY()

	AProjectorInteract();

	// Overrides the OnInteraction function, becauase blueprint native event
	void OnInteraction_Implementation(AFirstPersonCharacter*);
	
private:
	UMaterial* TheMaterial;
	UMaterialInstanceDynamic* TheMaterial_Dyn;
	TArray <UMaterialExpression*> BaseColorMatChain;
};
