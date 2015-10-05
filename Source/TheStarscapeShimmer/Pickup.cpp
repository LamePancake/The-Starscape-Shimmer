// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "Pickup.h"


// Sets default values
APickup::APickup()
{
 	
}

// Calls the pickup method if it is a pick up object
void APickup::OnInteraction_Implementation()
{
	Super::OnInteraction_Implementation();
	OnPickUp_Implementation();
}

void APickup::SetLocation_Implementation(FVector loc, FRotator rot)
{
	this->SetActorLocation(loc);
	this->SetActorRotation(rot);
	this->SetActorEnableCollision(false);
}

void APickup::OnPickUp_Implementation()
{
	//Empty, built upon by children
}

void APickup::OnDrop_Implementation()
{
	this->SetActorEnableCollision(true);
	bIsActive = true;
	Destroy();
}

