// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup.h"
#include "NotePickup.generated.h"

/**
 * 
 */
UCLASS()
class THESTARSCAPESHIMMER_API ANotePickup : public APickup
{
	GENERATED_BODY()
	
public:
	ANotePickup();

	void Read();

	// Background for when the player reads a note or enters the safe code
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Note)
	UTexture2D* NoteImage;

	// The index into the note array for the note
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Note)
	int32 NoteIndex;

	float Fade;

	void OnDrop_Implementation();

	static const FString Notes[];

	void Tick(float DeltaTime);
};
