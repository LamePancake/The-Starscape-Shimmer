// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "Pickup.h"


// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bIsActive = true;
	BaseCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("BaseSphereComponet"));

	// Set the SphereComponent as the root component
	RootComponent = BaseCollisionComponent;

	// Create the static mesh component
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));

	//Turn physics on for the static mesh
	PickupMesh->SetSimulatePhysics(false);

	// Attach static mesh component to the root component
	PickupMesh->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickup::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void APickup::OnPickUp_Implementation()
{
	// Other classes build upon this function
	// No function
	bIsActive = true;
}

