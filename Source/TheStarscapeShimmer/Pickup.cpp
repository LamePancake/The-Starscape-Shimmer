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
	this->SetActorEnableCollision(false);
	this->ObjectMesh->SetEnableGravity(false);
	this->ObjectMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->SetActorLocation(loc);
	this->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
	this->SetActorRotation(CurrentRotation);
	this->AddActorWorldRotation(rot);
	//this->SetActorRotation(rot);
}

void APickup::OnPickUp_Implementation()
{
	//Empty, built upon by children
}

void APickup::OnDrop_Implementation()
{
	this->SetActorEnableCollision(true);
	this->ObjectMesh->SetEnableGravity(true);
	this->ObjectMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	bIsActive = true;
	CurrentRotation = FRotator(0.0f, 0.0f, 0.0f);
}

void APickup::RotateObject(FRotator rot)
{
	CurrentRotation.Add(rot.Pitch, rot.Yaw, rot.Roll);
	//this->AddActorWorldRotation(rot);
}
