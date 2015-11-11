// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractableObject.h"
#include "SafeInteract.generated.h"

/**
 * 
 */
UCLASS()
class THESTARSCAPESHIMMER_API ASafeInteract : public AInteractableObject
{
	GENERATED_BODY()
public:

	ASafeInteract();

	// Unlocks the door if the right key is passed to it.
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Safe)
	void UnlockSafe();

	// The code to unlock the safe
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Safe)
	FString Code;

	// Tells us if the safe is locked
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Safe)
	bool IsLocked;
	
	// Will hook these up later
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
	AAmbientSound* SafeSpeaker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
	AAmbientSound* SafeUnlockSpeaker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Safe)
	bool EnteringCombination;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Safe)
	FString TestCombination;

	int CombinationLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Safe)
	AFirstPersonCharacter* CharacterReference;

	// Overrides the OnInteraction function, becauase blueprint native event
	void OnInteraction_Implementation(AFirstPersonCharacter*);
	
	bool EnterCombination();

	void Tick(float DeltaTime);

	bool CheckInput();

	float Fade;
};
