// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "FirstPersonCharacter.generated.h"

UCLASS(config = Game)
class THESTARSCAPESHIMMER_API AFirstPersonCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;
public:
	// Sets default values for this character's properties
	AFirstPersonCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	// Called every frame to update custom logic
	virtual void Tick(float deltaTime);

	// A sphere around the character used to tell if something is close enough to clollect.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pickup)
	USphereComponent* PickUpSphere;

protected:
	void OnFire();

	/* Handles backward & forward movement. */
	void MoveForward(float Val);

	/* Handles right & left movement. */
	void MoveRight(float Val);

	/* Turn left & right at a given rate. */
	void TurnAtRate(float Rate);

	/* Look up & down at a given rate. */
	void LookUpAtRate(float Rate);

	class APickup* GetPickupInView();

	// Trys to pick up object within it's sphere when key pressed
	UFUNCTION(BlueprintCallable, Category = Pickup)
	void PickUp();
};
