// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "AGameFootStepEffect.generated.h"

UCLASS()
class THESTARSCAPESHIMMER_API AAGameFootStepEffect : public AActor
{
	GENERATED_BODY()
public:
	/** default Footstep sound used when material specific override doesn't exist */
	UPROPERTY(EditDefaultsOnly, Category = Defaults)
		USoundCue* DefaultSound;

	/** Footstep FX on concrete */
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		USoundCue* ConcreteSound;

	/** Footstep FX on carpet */
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		USoundCue* CarpetSound;

	/** Footstep FX on wood */
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		USoundCue* WoodSound;

	/** surface data for spawning */
	UPROPERTY(BlueprintReadOnly, Category = Surface)
	FHitResult SurfaceHit;

	/** spawn effect */
	virtual void PostInitializeComponents() override;


	// Sets default values for this actor's properties
	AAGameFootStepEffect();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** get sound for material type */
	USoundCue* GetFootstepSound(TEnumAsByte<EPhysicalSurface> SurfaceType);

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;



};
