// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "ConsoleInteract.h"


// Sets default values
AConsoleInteract::AConsoleInteract()
{

}

// Calls the interact method
void AConsoleInteract::OnInteraction_Implementation()
{
	Super::OnInteraction_Implementation();
	OnInteract_Implementation();
}

void AConsoleInteract::OnInteract_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("HAHA, YOU CAN'T INTERACT BITCH"));
}

