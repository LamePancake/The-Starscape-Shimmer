// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "SliderInteract.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)

ASliderInteract::ASliderInteract()
{
	Initialized = false;
	Value = 0;

}

//Overrides the interation method
void ASliderInteract::OnInteraction_Implementation(AFirstPersonCharacter* Character)
{
	if (!Initialized)
	{
		PositionA = this->GetActorLocation();
		PositionB = FVector(PositionA.X + 10, PositionA.Y, PositionA.Z + 14);
		Translation = FVector(PositionB.X - PositionA.X, PositionB.Y - PositionA.Y, PositionB.Z - PositionA.Z);
		Initialized = true;
	}

	Value += 0.05f;

	if (Value > 1.0f) Value = 1.0f;

	this->SetActorLocation(FVector(PositionA.X + Translation.X * Value, PositionA.Y + Translation.Y * Value, PositionA.Z + Translation.Z * Value));
	print(PositionB.ToString());
}