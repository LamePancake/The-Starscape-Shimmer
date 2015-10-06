// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "ConsoleInteract.h"
#include "MovieScreenObject.h"


// Sets default values
AConsoleInteract::AConsoleInteract()
{
	// Get Reference to Movie Screen Material for Driller Killer
	/*static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Game/Content/Films/Driller_Killer/DrillerKillerUncit197_Tex_Mat.uasset'"));

	if (Material.Object != NULL)
	{
		TheMaterial = (UMaterial*)Material.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EMPTY MATERIAL"));
	}*/

	//TheMaterial_Dyn = UMaterialInstanceDynamic::Create(TheMaterial, this);

	/*static ConstructorHelpers::FObjectFinder<UBlueprint> MovieScreenObject(TEXT("BLUEPRINT'/Game/Content/Blueprints/BP_MovieScreenObject.uasset'"));

	if (MovieScreenObject.Object != NULL)
	{
		MovieScreen = (AMovieScreenObject*)MovieScreenObject.Object;
	}*/
}

// Calls the interact method
void AConsoleInteract::OnInteraction_Implementation()
{
	//Super::OnInteraction_Implementation();
	OnInteract_Implementation();
}

void AConsoleInteract::OnInteract_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("HAHA, YOU CAN'T INTERACT BITCH"));
	/*TheMaterial->GetExpressionsInPropertyChain(MP_BaseColor, BaseColorMatChain, NULL);
	for (int i = 0; i < BaseColorMatChain.Max(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("HueHueHueHueHue"));
	}*/

}

