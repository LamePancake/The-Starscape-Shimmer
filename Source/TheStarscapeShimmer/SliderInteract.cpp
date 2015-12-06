// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "SliderInteract.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)

ASliderInteract::ASliderInteract()
{
	Initialized = false;
	Value = 0.0;

}

//Overrides the interation method
void ASliderInteract::OnInteraction_Implementation(AFirstPersonCharacter* Character)
{
	if (!Initialized)
	{
		PositionA = this->GetActorLocation();
		PositionB = FVector(PositionA.X + XOffset, PositionA.Y, PositionA.Z + ZOffset);
		Translation = FVector(PositionB.X - PositionA.X, PositionB.Y - PositionA.Y, PositionB.Z - PositionA.Z);
		Initialized = true;
	}

	Value += Delta;

	if (Value > Max) Value = Max;

	this->SetActorLocation(FVector(PositionA.X + Translation.X * Value, PositionA.Y + Translation.Y * Value, PositionA.Z + Translation.Z * Value));
}

//Overrides the interation method
void ASliderInteract::OnAltInteraction_Implementation(AFirstPersonCharacter* Character)
{
	if (!Initialized)
	{
		PositionA = this->GetActorLocation();
		PositionB = FVector(PositionA.X + XOffset, PositionA.Y, PositionA.Z + ZOffset);
		Translation = FVector(PositionB.X - PositionA.X, PositionB.Y - PositionA.Y, PositionB.Z - PositionA.Z);
		Initialized = true;
	}

	Value -= Delta;

	if (Value < Min) Value = Min;

	this->SetActorLocation(FVector(PositionA.X + Translation.X * Value, PositionA.Y + Translation.Y * Value, PositionA.Z + Translation.Z * Value));
}