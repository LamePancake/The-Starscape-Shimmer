// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/Engine/Classes/Kismet/KismetMaterialLibrary.h"
#include "Runtime/Engine/Classes/Materials/MaterialParameterCollectionInstance.h"
#include "InteractableObject.h"
#include "SliderInteract.h"
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

	void Tick(float DeltaTime);

	// Overrides the OnInteraction function, becauase blueprint native event
	void OnInteraction_Implementation(AFirstPersonCharacter*);

	void CurrentFilm_Implementation(FString reelName);

	UPROPERTY() UMaterialParameterCollection* ScreenMatParams;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Screen)
		AAmbientSound* ConsoleSpeaker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Console)
		ASliderInteract* SliderA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Console)
		ASliderInteract* SliderB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Console)
		ASliderInteract* SliderC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Console)
		ASliderInteract* SliderD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Console)
		FString filmName;
};
