// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/StaticMeshActor.h"
#include "ProceduralStaticMeshActor.generated.h"

/**
 * 
 */
UCLASS()
class THESTARSCAPESHIMMER_API AProceduralStaticMeshActor : public AStaticMeshActor
{
	GENERATED_BODY()
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

	AProceduralStaticMeshActor();

private:
	double ElapsedTime;
	UTexture2D* DynamicTexture;
	UMaterialInstanceDynamic* DynamicMat;
	uint8* DynamicColours;
	uint32 DataSize;
	uint32 DataSizeSqrt;
	FVector Bounds;
	FUpdateTextureRegion2D* UpdateTextureRegion;
};
