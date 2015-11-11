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

	// The index into the note array for the note
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Note)
	int32 NoteIndex;

	float Fade;

	void OnDrop_Implementation();

	static const FString Notes[];

	void Tick(float DeltaTime);
};
