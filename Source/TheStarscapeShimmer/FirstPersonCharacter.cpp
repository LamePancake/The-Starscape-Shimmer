// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "FirstPersonCharacter.h"
#include "GameFramework/InputSettings.h"
#include "Pickup.h"
#include "KeyPickup.h"
#include "DoorObject.h"
#include "ProjectorInteract.h"
#include "FilmReelPickup.h"

// Sets default values
AFirstPersonCharacter::AFirstPersonCharacter()
{
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

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

	HeldItem = NULL;
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
}


void AFirstPersonCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AFirstPersonCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
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

void AFirstPersonCharacter::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (HeldItem != NULL)
	{
		FVector CamLoc;
		FRotator CamRot;
		float MaxUseDistance = 75.0f;

		Controller->GetPlayerViewPoint(CamLoc, CamRot);
		FVector Direction = CamRot.Vector();
		Direction.Normalize();
		FVector ItemLoc = CamLoc + (Direction * MaxUseDistance);

		HeldItem->SetLocation(ItemLoc, CamRot);
	}
	//APickup* Usable = GetPickupInView();
	//if (Usable)
	//{
		//UE_LOG(LogTemp, Warning, TEXT("Attempting pick up"));
		//Usable->OnPickUp();
	//}
}

// TODO: Cast a ray for pickup
void AFirstPersonCharacter::Grip()
{
	// Stores and retrieves actors with in the character's sphere
	TArray<AActor*> CollectableActors;
	PickUpSphere->GetOverlappingActors(CollectableActors);

	if (HeldItem)
	{
		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters s;
			s.Owner = this;
			s.Instigator = Instigator;
			World->SpawnActor<AKeyPickup>(ActorToSpawn, HeldItem->GetActorLocation(), HeldItem->GetActorRotation(), s);
		}
		HeldItem->OnDrop();
		HeldItem = nullptr;
		return;
	}

	APickup* TestPickup = nullptr;
	// Go through all of the actors
	for (int i = 0; i < CollectableActors.Num(); i++)
	{
		// If the item can be picked up, pick it up and set HeldItem to be the itme
		TestPickup = Cast<APickup>(CollectableActors[i]);
		if (TestPickup && !TestPickup->IsPendingKill() && TestPickup->bIsActive && HeldItem == NULL)
		{
			this->HeldItem = TestPickup;
			return;
		}
	}
}

// TODO: Cast a ray for interaction
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
			TestObj->OnInteraction(this);
			return;
		}
	}
}

APickup* AFirstPersonCharacter::GetPickupInView()
{
	FVector CamLoc;
	FRotator CamRot;

	float MaxUseDistance = 5.0f;

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
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f);

	return Cast<APickup>(Hit.GetActor());
}