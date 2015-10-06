// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractableObject.h"
#include "TheatreScreen.h"
//#include "Runtime/MediaAssets/Public/MediaPlayer.h"
#include "ProjectorInteract.generated.h"


/**
 * 
 */
UCLASS()
class THESTARSCAPESHIMMER_API AProjectorInteract : public AInteractableObject
{
	GENERATED_BODY()
	
public:

	AProjectorInteract()
	{
		//PublicDependencyModuleNames.AddRange(new string[] { "MediaAssets"});
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Materials)
		UMaterial* FilmMaterial;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Materials)
	//	UMediaPlayer* Film;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Screen)
		ATheatreScreen* TheatreScreen;


	void RunFilm()
	{
		TArray<UActorComponent*> components;

		TheatreScreen->GetComponents(components);
		for (int i = 0; i < components.Num(); ++i) //Because there may be more components
		{
			UStaticMeshComponent* thisComponent = Cast<UStaticMeshComponent>(components[i]); //try to cast to static mesh component
			if (thisComponent)
			{
				//This is the static mesh component
				thisComponent->SetMaterial(0, FilmMaterial);
				//Film->Play();
			}
		}
	}

/*public:
	// Sets default values for this actor's properties
	//AConsoleInteract();

	// Overrides the OnInteraction function, becauase blueprint native event
	void OnInteraction_Implementation();

	// Function to call when the pick up is interacted with
	UFUNCTION(BlueprintNativeEvent)
		void OnInteract();

private:
	/*UMaterial* TheMaterial;
	UMaterialInstanceDynamic TheMaterial_Dyn;
	TArray <UMaterialExpression*> BaseColorMatChain;*/
};
