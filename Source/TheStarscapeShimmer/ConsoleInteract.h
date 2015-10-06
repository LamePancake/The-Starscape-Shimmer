// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/Engine/Classes/Kismet/KismetMaterialLibrary.h"
#include "Runtime/Engine/Classes/Materials/MaterialParameterCollectionInstance.h"
#include "InteractableObject.h"
#include "ConsoleInteract.generated.h"

class THESTARSCAPESHIMMER_API AMovieScreenObject;

/**
 * 
 */
UCLASS()
class THESTARSCAPESHIMMER_API AConsoleInteract : public AInteractableObject
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AConsoleInteract();

	// Overrides the OnInteraction function, becauase blueprint native event
	void OnInteraction_Implementation(AFirstPersonCharacter*);

	UPROPERTY() AMovieScreenObject* MovieScreen;

	UPROPERTY() UMaterialParameterCollection* ScreenMatParams;

	//AMovieScreenObject* MovieScreenDefObj = AMovieScreenObject::GetClass()->GetDefaultObj<AMovieScreenObject>();
};
