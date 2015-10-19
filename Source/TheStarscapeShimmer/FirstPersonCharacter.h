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

	/** spawn effects for footsteps */
	void SpawnFootEffects();

	/** This is the template for our footsteps */
	UPROPERTY(Category = "Game Effects", EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AAGameFootStepEffect> FootstepTemplate;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	/** Rate at which footsteps play, larger == less often. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Footsteps)
	int32 FootstepRate;

	int32 FootstepTimer;

	/** Base rate that the object rotates at. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseRotateRate;

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

	// Item held by the player
	class APickup* HeldItem;

	// Item that the player is looking at
	class AInteractableObject* LookAtItem;

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

	/* Update step timer and play sounds accordingly. */
	void UpdateFootsteps();

	// Rotates the object that is being held about the x axis
	void RotateObjectX(float Val);
	
	// Rotates the object that is being held about the y axis
	void RotateObjectY(float Val);

	// Checks to see if the object is in view of the player or not
	class AInteractableObject* Trace();

	// Checks to see if the object is in view of the player or not
	bool CheckInView(class AInteractableObject* o);

	// Tries to pick up any nearby pickuppable™ objects
	// TODO: Rename this piece of shit
	UFUNCTION(BlueprintCallable, Category = Interaction)
	void Grip();

	// Interacts with the object under the reticle if possible
	UFUNCTION(BlueprintCallable, Category = Interaction)
	void Interact();

	// Highlights the object that is in view of the raycast
	void HighlightObjectsInView();
};
