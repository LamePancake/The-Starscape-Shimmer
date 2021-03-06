// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "InteractableObject.h"


// Sets default values
AInteractableObject::AInteractableObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bIsActive = true;
	BaseCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("BaseSphereComponet"));

	// Set the SphereComponent as the root component
	RootComponent = BaseCollisionComponent;

	// Create the static mesh component
	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMesh"));

	// Attach static mesh component to the root component
	ObjectMesh->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void AInteractableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractableObject::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

//Overridable method to do something when the player interacts with it.
void AInteractableObject::OnInteraction_Implementation(AFirstPersonCharacter* Character)
{
	if (!bIsActive) return;
	// Other classes build upon this function
	// No function
}

//Overridable method to do something when the player interacts with it.
void AInteractableObject::OnAltInteraction_Implementation(AFirstPersonCharacter* Character)
{
	if (!bIsActive) return;
	// Other classes build upon this function
	// No function
}

// Turns on custom rendering to render the glow
void AInteractableObject::OnLookAt()
{
	if (!bIsActive) return;
	ObjectMesh->SetRenderCustomDepth(true);
}

// Turns off custom rendering to take away the glow
void AInteractableObject::OnLookAway()
{
	if (!bIsActive) return;
	ObjectMesh->SetRenderCustomDepth(false);
}


