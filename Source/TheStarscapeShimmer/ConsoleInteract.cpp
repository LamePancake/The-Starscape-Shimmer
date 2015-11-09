// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "ConsoleInteract.h"
//#include "MovieScreenObject.h"


// Sets default values
AConsoleInteract::AConsoleInteract()
{
	// Get Reference to Movie Screen Material Parameter Collection

	static ConstructorHelpers::FObjectFinder<UMaterialParameterCollection> ScrnMatParamColl(TEXT("MaterialParameterCollection'/Game/ScreenMatParams'"));
	if (ScrnMatParamColl.Object != NULL)
	{
		ScreenMatParams = (UMaterialParameterCollection*)ScrnMatParamColl.Object;
	}
}

// Calls the interact method
void AConsoleInteract::OnInteraction_Implementation(AFirstPersonCharacter* Character)
{
	Super::OnInteraction_Implementation(Character);

	UAudioComponent* SpeakerAudio = ConsoleSpeaker->GetAudioComponent();
	SpeakerAudio->Stop();
	SpeakerAudio->Play();
}

void AConsoleInteract::Tick(float DeltaTime)
{
	UKismetMaterialLibrary::SetScalarParameterValue(this, ScreenMatParams, FName(TEXT("SliderA")), SliderA->GetValue());
	UKismetMaterialLibrary::SetScalarParameterValue(this, ScreenMatParams, FName(TEXT("SliderB")), SliderB->GetValue());
	UKismetMaterialLibrary::SetScalarParameterValue(this, ScreenMatParams, FName(TEXT("SliderC")), SliderC->GetValue());
	UKismetMaterialLibrary::SetScalarParameterValue(this, ScreenMatParams, FName(TEXT("SliderD")), SliderD->GetValue());
}

void AConsoleInteract::CurrentFilm_Implementation(FString reelName) 
{
	filmName = reelName;
}
