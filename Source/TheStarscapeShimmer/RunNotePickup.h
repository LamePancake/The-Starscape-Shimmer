// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NotePickup.h"
#include "RunNotePickup.generated.h"

/**
 * 
 */
UCLASS()
class THESTARSCAPESHIMMER_API ARunNotePickup : public ANotePickup
{
	GENERATED_BODY()
	
public:
	ARunNotePickup();
	virtual ~ARunNotePickup();

	virtual void Read();

	UFUNCTION(BlueprintCallable, Category=Object)
	bool RunNoteWasRead();

private:
	static int InstanceCount;
	static bool WasRead;
};
