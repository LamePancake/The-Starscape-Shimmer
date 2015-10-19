// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "AGameFootStepEffect.h"

#define GAME_SURFACE_Default    SurfaceType_Default
#define GAME_SURFACE_Wood   SurfaceType1
#define GAME_SURFACE_Carpet  SurfaceType2
#define GAME_SURFACE_Concrete   SurfaceType3


#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)

// Sets default values
AAGameFootStepEffect::AAGameFootStepEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

}

// Called when the game starts or when spawned
void AAGameFootStepEffect::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAGameFootStepEffect::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//SurfaceHit was set during the spawn of this actor. We get the PhysMaterial here
	UPhysicalMaterial* HitPhysMat = SurfaceHit.PhysMaterial.Get();
	EPhysicalSurface HitSurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitPhysMat);
	
	// play sound
	USoundCue* FootstepSound = GetFootstepSound(HitSurfaceType);
	if (FootstepSound)
	{
		//print(TEXT("sound out!"));
		UGameplayStatics::PlaySoundAtLocation(this, FootstepSound, GetActorLocation());
	}
}

USoundCue* AAGameFootStepEffect::GetFootstepSound(TEnumAsByte<EPhysicalSurface> SurfaceType)
{
	USoundCue* FootstepSound = NULL;
	switch (SurfaceType)
	{
		case GAME_SURFACE_Concrete:
		{
			FootstepSound = ConcreteSound;
			//print(TEXT("concrete")); 
		} break;
		case GAME_SURFACE_Carpet:
		{
			FootstepSound = CarpetSound; 	
			//print(TEXT("carpet"));
		} break;
		case GAME_SURFACE_Wood:
		{
			FootstepSound = CarpetSound; 	
			//print(TEXT("wood"));
		} break;
		case GAME_SURFACE_Default:
		{
			FootstepSound = DefaultSound; 	
			//print(TEXT("default"));
		} break;
		default:
		{
			FootstepSound = NULL;
			//print(TEXT("unknown"));
		} break;
	}

	return FootstepSound ? FootstepSound : DefaultSound;
}

void AAGameFootStepEffect::Tick(float DeltaSeconds)
{
	//Destroy the emiter after 2.5 seconds
	Super::Tick(DeltaSeconds);
	const float TimeAlive = GetWorld()->GetTimeSeconds() - CreationTime;
	if (TimeAlive > 2.5f)
	{
		Destroy(true);
	}
}

