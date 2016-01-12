// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "ProceduralFunc.h"
#include <atomic>
#include "Engine/StaticMeshActor.h"

class FProceduralRunnable;

/**
 * 
 */
class THESTARSCAPESHIMMER_API ProceduralThreadPool
{
public:
	// Get the current instance of the thread pool
	static ProceduralThreadPool* GetInstance();

	/// <description>
	/// Call this method when you won't need this object anymore (probably in a destructor or similar).
	/// </description>
	void Release();

	/// <description>
	/// Generates the texture using the initially supplied procedure and the stores it in the given byte array.
	/// </description>
	void DoGeneration(uint8* Data, FVector* Bounds, double ElapsedTime, ProceduralFunc Function);

	inline uint32 GetChunkOffset(int Index) const
	{
		return ChunkOffsets[Index];
	}

	inline size_t GetChunkSize(int Index) const
	{
		return ChunkSizes[Index];
	}

	inline uint8* GetData() const
	{
		return Data;
	}

	inline FVector* GetBounds() const
	{
		return Bounds;
	}

	inline double GetElapsedTime() const
	{
		return ElapsedTime;
	}

	inline ProceduralFunc GetProceduralFunc() const
	{
		return Function;
	}

	// Allows the game thread to wait until all threads have completed
	std::atomic<unsigned int> CompletionCounter;

	// The variable that signals when to start
	std::atomic<bool> ShouldGenerate;

	// Array to hold whether a given thread has finished execution yet
	std::atomic<bool> *Finished;

private:
	ProceduralThreadPool(uint32 ThreadCount);
	~ProceduralThreadPool();

	// The number of threads we'll use to generate the 
	uint32 NumThreads;
	
	FRunnableThread** Threads;

	// The runnable objects that handle the content generation
	FProceduralRunnable** Runnables;

	// The function used to generate the pixels for a given invocation of DoGeneration
	ProceduralFunc Function;

	// An array holding the pointers into the colours array for each thread 
	uint32* ChunkOffsets;

	// Holds the size of each chunk
	size_t* ChunkSizes;

	// The colour data that will be operated on
	uint8* Data;

	// The bounds of the currently processed object
	FVector* Bounds;

	double ElapsedTime;
};
