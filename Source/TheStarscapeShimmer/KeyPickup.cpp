// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "KeyPickup.h"


AKeyPickup::AKeyPickup()
{
	// Key does not work with any door
	WhichDoor = -1.0f;
}

//Handles the pick up if it is a key
void AKeyPickup::OnPickUp_Implementation()
{
	Super::OnPickUp_Implementation();
	UE_LOG(LogTemp, Warning, TEXT("HAHA, YOU CAN'T PICK ME UP"));
}