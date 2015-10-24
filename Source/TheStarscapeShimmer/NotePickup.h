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
	void Read();

	// The String the note holds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Note)
	FString Note;

	void OnDrop_Implementation();
};
