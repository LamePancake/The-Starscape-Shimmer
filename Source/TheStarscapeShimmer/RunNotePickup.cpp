// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "RunNotePickup.h"

bool ARunNotePickup::WasRead = false;
int ARunNotePickup::InstanceCount = 0;

ARunNotePickup::ARunNotePickup() {
	ARunNotePickup::InstanceCount++;
}

ARunNotePickup::~ARunNotePickup() {
	ARunNotePickup::InstanceCount--;
	if (ARunNotePickup::InstanceCount == 0) {
		ARunNotePickup::WasRead = false;
	}
}

void ARunNotePickup::Read() {
	Super::Read();
	if (!ARunNotePickup::WasRead) {
		ARunNotePickup::WasRead = true;
	}
}

bool ARunNotePickup::RunNoteWasRead() {
	return WasRead;
}
