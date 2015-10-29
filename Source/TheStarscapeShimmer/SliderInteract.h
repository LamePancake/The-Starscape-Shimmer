// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractableObject.h"
#include "SliderInteract.generated.h"

/**
 * 
 */
UCLASS()
class THESTARSCAPESHIMMER_API ASliderInteract : public AInteractableObject
{
public:
	GENERATED_BODY()

	ASliderInteract();
	
	void OnInteraction_Implementation(AFirstPersonCharacter* Character);
	void OnAltInteraction_Implementation(AFirstPersonCharacter* Character);

	float GetValue() { return Value; }
protected:
	bool Initialized;
	FVector PositionA;
	FVector PositionB;
	FVector Translation;
	float Value;
};
