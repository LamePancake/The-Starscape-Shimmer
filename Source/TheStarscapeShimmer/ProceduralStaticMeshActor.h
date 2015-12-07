// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <cstdint>
#include "ProceduralFunc.h"
#include "Engine/StaticMeshActor.h"
#include "ProceduralStaticMeshActor.generated.h"

#define NUM_THREADS 3

/**
 * 
 */
UCLASS()
class THESTARSCAPESHIMMER_API AProceduralStaticMeshActor : public AStaticMeshActor
{
	GENERATED_BODY()
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	virtual ~AProceduralStaticMeshActor();

	AProceduralStaticMeshActor();

private:
	// The amount of time since the game started running
	double ElapsedTime;

	// A texture object that we'll change over time
	UTexture2D* DynamicTexture;

	// The material which will be updated with our texture
	UMaterialInstanceDynamic* DynamicMat;

	// An array to which we can write new colour values
	uint8* DynamicColours;

	// The size, in bytes, of the dynamic colour array
	uint32_t DataSize;

	// The square root of that size
	uint32_t DataSizeSqrt;

	// The actor's dimensions
	FVector Bounds;

	// A scaled down version of the actor's dimensions suitable for generating a texture (hopefully...)
	FVector TextureBounds;

	// The texture region to update (will be the entire thing)
	FUpdateTextureRegion2D* UpdateTextureRegion;

	// An array holding the pointers into the colours array for each thread 
	uint32_t ChunkOffsets[NUM_THREADS];

	// Holds the size of each chunk
	size_t ChunkSizes[NUM_THREADS];
};
