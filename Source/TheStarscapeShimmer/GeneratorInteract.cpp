// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "GeneratorInteract.h"

// Sets default values
AGeneratorInteract::AGeneratorInteract()
{

}

//Overrides the interation method
void AGeneratorInteract::OnInteraction_Implementation()
{
	Super::OnInteraction_Implementation();
	//Play sound here
	//Turn lights on here
	UE_LOG(LogTemp, Warning, TEXT("Your message"));
}


