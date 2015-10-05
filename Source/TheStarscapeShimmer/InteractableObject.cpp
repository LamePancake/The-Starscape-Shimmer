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

	//Turn physics on for the static mesh
	ObjectMesh->SetSimulatePhysics(false);

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
void AInteractableObject::OnInteraction_Implementation()
{
	// Other classes build upon this function
	// No function
}


