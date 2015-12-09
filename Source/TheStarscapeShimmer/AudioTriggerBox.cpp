// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "AudioTriggerBox.h"

AAudioTriggerBox::AAudioTriggerBox() {
	//Speaker = GetWorld()->SpawnActor<AAmbientSound>(AAmbientSound::StaticClass());
}

void AAudioTriggerBox::BeginPlay() {
	//Speaker->SeActorLocation(GetActorLocation());
	//Speaker->AudioComponent->SetSound(ShimmerVoiceClips[0]);
}


void AAudioTriggerBox::Tick(float DeltaTime) {

}

void AAudioTriggerBox::Speak() {

}
