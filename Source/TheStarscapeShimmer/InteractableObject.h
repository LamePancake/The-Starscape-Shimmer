// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "FirstPersonCharacter.h"
#include "InteractableObject.generated.h"

UCLASS()
class THESTARSCAPESHIMMER_API AInteractableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableObject();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// True when the pickup is able to be picked up, false if something deactivates the pickup
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Object)
	bool bIsActive;

	// Simple collision primitive to use as the root component
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Object)
	USphereComponent* BaseCollisionComponent;

	// Represents the Interactable obejct in the level
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Object)
	UStaticMeshComponent* ObjectMesh;

	// Function to call when the player interacts with something
	UFUNCTION(BlueprintNativeEvent)
	void OnInteraction(AFirstPersonCharacter* Character);

	// Function to call when the player interacts with something
	UFUNCTION(BlueprintNativeEvent)
	void OnAltInteraction(AFirstPersonCharacter* Character);

	// Called when the player is looking at this object
	void OnLookAt();

	// Called when the player is not looking at the object anymore
	void OnLookAway();
};
