// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class THESTARSCAPESHIMMER_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// True when the pickup is able to be picked up, false if something deactivates the pickup
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
	bool bIsActive;

	// Simple collision primitive tto use as the root component
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Pickup)
	USphereComponent* BaseCollisionComponent;

	// Represents the pickip in the level
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Pickup)
	UStaticMeshComponent* PickupMesh;

	// Function to call when the pick up is collected
	UFUNCTION(BlueprintNativeEvent)
	void OnPickUp();
	
};
