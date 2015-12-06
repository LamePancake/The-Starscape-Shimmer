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

	UPROPERTY(EditAnywhere, Category = Slide)
		int32 XOffset = 10;

	UPROPERTY(EditAnywhere, Category = Slide)
		int32 ZOffset = 14;

	UPROPERTY(EditAnywhere, Category = Slide)
		float Delta = 0.1f;

	UPROPERTY(EditAnywhere, Category = Slide)
		float Max = 1.0f;

	UPROPERTY(EditAnywhere, Category = Slide)
		float Min = 0.0f;
	
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
