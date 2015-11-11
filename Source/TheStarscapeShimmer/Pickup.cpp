// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "Pickup.h"


// Sets default values
APickup::APickup()
{
	CurrentRotation = FRotator(0.0f, 0.0f, 0.0f);
}

// Calls the pickup method if it is a pick up object
void APickup::OnInteraction_Implementation(AFirstPersonCharacter* Character)
{
	Super::OnInteraction_Implementation(Character);
	OnPickUp_Implementation();
}

void APickup::SetLocation_Implementation(FVector loc, FRotator rot)
{
	this->ObjectMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	this->ObjectMesh->SetEnableGravity(false);
	this->SetActorLocation(loc);
	this->SetActorRotation(CurrentRotation);
	this->AddActorWorldRotation(rot);
}

void APickup::OnPickUp_Implementation()
{
	if (!bIsActive) return;
	// Empty, built upon by children
}

void APickup::OnDrop_Implementation()
{
	this->ObjectMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	this->ObjectMesh->SetEnableGravity(true);
	bIsActive = true;
	CurrentRotation = FRotator(0.0f, 0.0f, 0.0f);
}

void APickup::RotateObject(FRotator rot)
{
	CurrentRotation.Add(rot.Pitch, rot.Yaw, rot.Roll);
	//this->AddActorWorldRotation(rot);
}
