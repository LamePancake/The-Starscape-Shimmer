// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "FirstPersonCharacter.h"
#include "GameFramework/InputSettings.h"
#include "Pickup.h"
#include "KeyPickup.h"
#include "DoorObject.h"
#include "AGameFootStepEffect.h"
#include "ProjectorInteract.h"
#include "FilmReelPickup.h"
#include "NotePickup.h"

// Sets default values
AFirstPersonCharacter::AFirstPersonCharacter()
{
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// setup footsteps
	FootstepTimer = 0;

	Walking = false;

	// Set your rotate rate for held objects
	BaseRotateRate = 3.0f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->AttachParent = GetCapsuleComponent();
	FirstPersonCameraComponent->RelativeLocation = FVector(0, 0, 54.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create and attach the sphere component to the player
	PickUpSphere = CreateDefaultSubobject <USphereComponent> (TEXT("PickUp Sphere"));
	PickUpSphere->AttachTo(RootComponent);
	PickUpSphere->SetSphereRadius(100.0f);

	//Turn on the update method
	PrimaryActorTick.bCanEverTick = true;

	// Set the current held item to nothing
	HeldItem = NULL;

	// Set the item currently being looked at to nothing
	LookAtItem = NULL;
}

void AFirstPersonCharacter::SpawnFootEffects()
{
	//Rotation of the ray mesh (Horizontal and Vertical)
	FRotator Rotation;
	FHitResult Hit(ForceInit);
	FVector Start, End;
	static FName FootStepTag = FName(TEXT("FootStepTrace"));
	FCollisionQueryParams TraceParams(FootStepTag, false, GetOwner());
	//We need the ground physical material to be returned so this is very important
	TraceParams.bReturnPhysicalMaterial = true;
	if (FootstepTemplate)
	{
		Controller->GetPlayerViewPoint(Start, Rotation);

		// Offset so that the character capsule doesn't interfer... :(
		Start.X -= 20.0;
		Start.Y -= 20.0;
		Start.Z -= 150.0;
		//The End point will be somewhere under the foot
		End = Start + (FVector::FVector(0.0f, 0.0f, -1.0f) * 10);
		//After this, Hit has everything we need to know about the ground
		GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_EngineTraceChannel1, TraceParams);

		//SpawnActorDeferred allow us to set variables before finishing spawning the actor.
		FVector Scale(1, 1, 1);
		FTransform Transform(Rotation, GetActorLocation(), Scale);
		AAGameFootStepEffect* EffectActor = GetWorld()->SpawnActorDeferred<AAGameFootStepEffect>(FootstepTemplate, Transform);
		if (EffectActor)
		{
			//The physic material is stored in the Hit result, we set it in the effect actor so we can know what effect to spawn
			EffectActor->SurfaceHit = Hit;
			//Ok, our actor knows where (Hit.Location) to spawn, go on
			UGameplayStatics::FinishSpawningActor(EffectActor, FTransform(Rotation, Hit.Location));
		}
	}
}

// Called when the game starts or when spawned
void AFirstPersonCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called to bind functionality to input
void AFirstPersonCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	// set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &AFirstPersonCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AFirstPersonCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &AFirstPersonCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &AFirstPersonCharacter::LookUpAtRate);

	// Bind the key press of E or Right face button for picking up object.
	InputComponent->BindAction("Pickup", IE_Pressed, this, &AFirstPersonCharacter::Grip);
	InputComponent->BindAction("Interaction", IE_Pressed, this, &AFirstPersonCharacter::Interact);

	// Bind the key press for E to read the a note
	InputComponent->BindAction("Read", IE_Pressed, this, &AFirstPersonCharacter::ReadNote);

	// Bind the axis for rotating the held objects
	InputComponent->BindAxis("RotateObjectX", this, &AFirstPersonCharacter::RotateObjectY);
	InputComponent->BindAxis("RotateObjectY", this, &AFirstPersonCharacter::RotateObjectX);
}

void AFirstPersonCharacter::UpdateFootsteps(float delta)
{
	FootstepTimer += delta;

	if (FootstepTimer >= FootstepRate)
	{
		SpawnFootEffects();
		FootstepTimer = 0;
	}
}

void AFirstPersonCharacter::MoveForward(float Value)
{
	// Make sure the player is not holding the right mouse button
	APlayerController* PlayerController = Cast<APlayerController>(this->GetController());
	if (PlayerController)
	{
		if (!PlayerController->IsInputKeyDown(EKeys::RightMouseButton))
		{
			if (Value != 0.0f)
			{
				// add movement in that direction
				AddMovementInput(GetActorForwardVector(), Value);
				Walking = true;
			}
		}
	}
}

void AFirstPersonCharacter::MoveRight(float Value)
{
	// Make sure the player is not holding the right mouse button
	APlayerController* PlayerController = Cast<APlayerController>(this->GetController());
	if (PlayerController)
	{
		if (!PlayerController->IsInputKeyDown(EKeys::RightMouseButton))
		{
			if (Value != 0.0f)
			{
				// add movement in that direction
				AddMovementInput(GetActorRightVector(), Value);
				Walking = true;
			}
		}
	}
}

void AFirstPersonCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFirstPersonCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

// Rotates the held object about the x axis when the a or d key and the right mouse button are pressed
void AFirstPersonCharacter::RotateObjectX(float Val)
{
	if (HeldItem != NULL)
	{
		// Make sure the player is holding the right mouse button
		APlayerController* PlayerController = Cast<APlayerController>(this->GetController());
		if (PlayerController)
		{
			if (PlayerController->IsInputKeyDown(EKeys::RightMouseButton))
			{
				if (Val != 0.0f)
				{
					// rotate the object about the x axis
					FRotator r = FRotator(Val * BaseRotateRate, 0.0f, 0.0f);
					HeldItem->RotateObject(r);
				}	
			}
		}
	}
}

// Rotates the held object about the y axis when the w or s key and the right mouse button are pressed
void AFirstPersonCharacter::RotateObjectY(float Val)
{
	if (HeldItem != NULL)
	{
		// Make sure the player is holding the right mouse button
		APlayerController* PlayerController = Cast<APlayerController>(this->GetController());
		if (PlayerController)
		{
			if (PlayerController->IsInputKeyDown(EKeys::RightMouseButton))
			{
				if (Val != 0.0f)
				{
					// rotate the object about the y axis
					FRotator r = FRotator(0.0f, Val * BaseRotateRate, 0.0f);
					HeldItem->RotateObject(r);
				}
			}
		}
	}
}

void AFirstPersonCharacter::ReadNote()
{
	if (HeldItem != NULL)
	{
		ANotePickup* TestNote = Cast<ANotePickup>(HeldItem);
		if (TestNote)
		{
			TestNote->Read();
		}
	}
}

void AFirstPersonCharacter::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (HeldItem != NULL)
	{
		FVector CamLoc;
		FRotator CamRot;
		float MaxUseDistance = 85.0f;

		Controller->GetPlayerViewPoint(CamLoc, CamRot);
		FVector Direction = CamRot.Vector();
		Direction.Normalize();
		FVector ItemLoc = CamLoc + (Direction * MaxUseDistance);

		HeldItem->SetLocation(ItemLoc, CamRot);
	}

	if (Walking)
	{
		UpdateFootsteps(deltaTime);
		Walking = false;
	}

	HighlightObjectsInView();
	
}

// TODO: Refactor and maybe call less frequently
void AFirstPersonCharacter::HighlightObjectsInView()
{
	AInteractableObject* itemTest = Trace();
	if (itemTest)
	{
		if (itemTest != LookAtItem)
		{
			if (LookAtItem)
				LookAtItem->OnLookAway();
			LookAtItem = NULL;
		}
		itemTest->OnLookAt();
		LookAtItem = itemTest;
	}
	else
	{
		if (LookAtItem)
			LookAtItem->OnLookAway();
	}
}

// Checks objects around the player to see if they can be picked up
void AFirstPersonCharacter::Grip()
{
	// Stores and retrieves actors with in the character's sphere
	TArray<AActor*> CollectableActors;
	PickUpSphere->GetOverlappingActors(CollectableActors);

	if (HeldItem)
	{
		HeldItem->OnDrop();
		HeldItem = nullptr;
		return;
	}

	//APickup* TestPickup = nullptr;
	// Go through all of the actors
	for (int i = 0; i < CollectableActors.Num(); i++)
	{
		// If the item can be picked up, pick it up and set HeldItem to be the itme
		APickup* TestPickup = Cast<APickup>(CollectableActors[i]);
		if (TestPickup && !TestPickup->IsPendingKill() && TestPickup->bIsActive && HeldItem == NULL)
		{
			if (CheckInView(TestPickup))
			{
				this->HeldItem = TestPickup;
				return;
			}
		}
	}
}

// Checks objects around the player to see if they can be interacted with
void AFirstPersonCharacter::Interact()
{
	// Stores and retrieves actors with in the character's sphere
	TArray<AActor*> CollectableActors;
	PickUpSphere->GetOverlappingActors(CollectableActors);

	// Go through all of the actors
	for (int i = 0; i < CollectableActors.Num(); i++)
	{
		// If it is an object, do what the object does.
		AInteractableObject* const TestObj = Cast<AInteractableObject>(CollectableActors[i]);
		if (TestObj && !TestObj->IsPendingKill() && TestObj->bIsActive ) 
		{
			if (CheckInView(TestObj))
			{
				TestObj->OnInteraction(this);
				return;
			}
		}
	}
}

// Returns an interactable object if the player is looking at one
AInteractableObject* AFirstPersonCharacter::Trace()
{
	FVector CamLoc;
	FRotator CamRot;

	float MaxUseDistance = 160.0f;

	if (Controller == NULL)
		return NULL;

	// This retrieves are camera point of view to find the start and direction we will trace. 
	Controller->GetPlayerViewPoint(CamLoc, CamRot);
	const FVector TraceStart = CamLoc;
	const FVector Direction = CamRot.Vector();
	const FVector TraceEnd = TraceStart + (Direction * MaxUseDistance);

	FCollisionQueryParams TraceParams(FName(TEXT("TraceUsableActor")), true, this);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.bTraceComplex = true;

	// FHitResults is passed in with the trace function and holds the result of the trace. 
	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, TraceParams);

	// Uncomment this to visualize your line during gameplay. 
	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f);

	return Cast<AInteractableObject>(Hit.GetActor());
}

// Checks to see if th player is looking directly at an object.
bool AFirstPersonCharacter::CheckInView(AInteractableObject* o)
{
	AInteractableObject* test = Trace();

	if (test)
	{
		if (test == o)
			return true;
	}

	return false;
}