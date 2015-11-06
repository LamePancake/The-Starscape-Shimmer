// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "TrigLookup.h"
#include "ProceduralStaticMeshActor.h"
#include "AllowWindowsPlatformTypes.h"
#include <atomic>
#include <cmath>
#include "HideWindowsPlatformTypes.h"

struct ProceduralWorkerThreads;
static void ProceduralFunction(ProceduralWorkerThreads*, int);

class FProceduralRunnable : public FRunnable {

public:
	FProceduralRunnable(const int Index, ProceduralWorkerThreads* Threads)
		: Index(Index), ThreadPool(Threads)
	{}

	// We've already done all necessary initialisation in the constructor
	virtual bool Init() override {
		return true;
	}

	// I didn't want to bother putting the function in here for now
	// TOOD: Move the function into here sometime from now
	virtual uint32 Run() override {
		ProceduralFunction(ThreadPool, Index);
		return 0;
	}

	virtual void Exit() override {
		// Lol don't do anything here
	}

	virtual void Stop() override {
		// Lol don't do anything here
	}

private:
	int Index;
	ProceduralWorkerThreads* ThreadPool;
};

struct ProceduralWorkerThreads {
	
	// Initialise the threads
	ProceduralWorkerThreads()
		: CompletionCounter(0), ChunkOffsets(nullptr), ChunkSizes(nullptr), ShouldStop(false), ShouldGenerate(false), ElapsedTime(0.0) {
		for (int i = 0; i < NUM_THREADS; i++) {
			FString ThreadName = FString::Printf(TEXT("ProcThread %d"), i);
			Finished[i] = true;
			Runnables[i] = new FProceduralRunnable(i, this);
			Threads[i] = FRunnableThread::Create(Runnables[i], ThreadName.GetCharArray().GetData(), 0, TPri_Normal);
		}
	}

	// TODO Clean up the rest of this stupid object
	virtual ~ProceduralWorkerThreads() {
		Stop();
		for (int i = 0; i < NUM_THREADS; i++) {
			delete Runnables[i];
		}
	}

	// Generates the texture
	void DoProceduralGeneration(uint8* Data, uint32_t* ChunkOffsets, size_t* ChunkSizes, FVector* Bounds, double ElapsedTime) {
		
		this->ChunkOffsets = ChunkOffsets;
		this->ChunkSizes = ChunkSizes;
		this->Data = Data;
		this->Bounds = Bounds;
		this->ElapsedTime = ElapsedTime;

		// Tell the threads to start
		for (int i = 0; i < NUM_THREADS; i++) {
			Finished[i] = false;
		}
		unsigned int Iterations = 0;
		bool AllFinished = false;

		for (int i = 0; i < NUM_THREADS; i++) {
			Finished[i] = false;
		}

		// Continue looping through finished array until every thread has finished
		while (!AllFinished && Iterations < 100000000) {
			AllFinished = true;
			for (int i = 0; i < NUM_THREADS; i++) {
				if (!Finished[i]) {
					AllFinished = false;
					break;
				}
			}
			Iterations++;
		}

		if (Iterations == 100000000) {
			UE_LOG(LogTemp, Warning, TEXT("Reached 100000000 iterations! AllFinished is %d"), AllFinished);
		}
	}

	uint32_t GetChunkOffset(int Index) const {
		return ChunkOffsets[Index];
	}

	size_t GetChunkSize(int Index) const {
		return ChunkSizes[Index];
	}

	uint8* GetData() const {
		return Data;
	}

	FVector* GetBounds() const {
		return Bounds;
	}

	double GetElapsedTime() const {
		return ElapsedTime;
	}

	// Stops the threads
	void Stop() {
		ShouldStop.store(true);
		for (int i = 0; i < NUM_THREADS; i++)
			Finished[i] = false;
	}

	// Allows the game thread to wait until all threads have completed
	std::atomic<unsigned int> CompletionCounter;

	// Whether we should stop running
	std::atomic<bool> ShouldStop;

	// The variable that signals when to start (and its associated lock)
	std::atomic<bool> ShouldGenerate;

	// Array to hold whether a given thread has finished execution yet
	std::atomic<bool> Finished[NUM_THREADS];
	
private:
	FRunnableThread* Threads[NUM_THREADS];

	// The runnable objects that handle the content generation
	FProceduralRunnable* Runnables[NUM_THREADS];

	// An array holding the pointers into the colours array for each thread 
	uint32_t* ChunkOffsets;

	// Holds the size of each chunk
	size_t* ChunkSizes;

	// The colour data that will be operated on
	uint8* Data;

	// The bounds of the currently processed object
	FVector* Bounds;

	double ElapsedTime;
};

static void ProceduralFunction(ProceduralWorkerThreads* ThreadPool, int Index) {
	while (1) {
		while (ThreadPool->Finished[Index]);

		// Since the ThreadPool's Data will change between invocations, this is necessary to do each time
		uint8* Data = ThreadPool->GetData();
		uint32_t Offset = ThreadPool->GetChunkOffset(Index);
		uint8* Chunk = Data + (Offset * 4);						// Each pixel is 4 bytes, so we have to skip ahead by offset * 4
		size_t Size = ThreadPool->GetChunkSize(Index);
		FVector Bounds = *ThreadPool->GetBounds();				// Make a copy in case we're supposed to stop later
		double ElapsedTime = ThreadPool->GetElapsedTime();

		for (uint32_t i = 0; i < Size && !ThreadPool->ShouldStop; i++) {
			uint32_t Y = (i + Offset) / (uint32_t)Bounds.Y;
			uint32_t Z = (i + Offset) % (uint32_t)Bounds.Y;

			double ShiftedY = (Y / Bounds.Y * 2) - 1.0;
			double ShiftedZ = (Z / Bounds.Z * 2) - 1.0;

			// Use the lookup tables, which are hopefully faster than computing the values
			float mov0 = ShiftedY + ShiftedZ + CosLookup(SinLookup(ElapsedTime) * 2.0) * 100.f + SinLookup(ShiftedY / 100.f) * 1000.f;
			float mov1 = ShiftedY / 0.9 + ElapsedTime;
			float mov2 = ShiftedY / 0.2;
			float c1 = fabs(SinLookup(mov1 + ElapsedTime) / 2.f + mov2 / 2.f - mov1 - mov2 + ElapsedTime);
			float c2 = fabs(SinLookup(c1 + SinLookup(mov0 / 1000.f + ElapsedTime) + SinLookup(ShiftedZ / 40.f + ElapsedTime) + SinLookup((ShiftedY + ShiftedZ) / 100.f) * 3.f));
			float c3 = fabs(SinLookup(c2 + CosLookup(mov1 + mov2 + c2) + CosLookup(mov2) + SinLookup(ShiftedY / 1000.f)));

			// TODO: Need some check here to make sure we don't write to the buffer if the thread stopped
			// *Theoretically*, nothing should be freed before we finish the loop, but I don't really trust that. At all.
			Chunk[i * 4] = (uint8)(c1 * 255);
			Chunk[(i * 4) + 1] = (uint8)(c2 * 255);
			Chunk[(i * 4) + 2] = (uint8)(c3 * 255);
			Chunk[(i * 4) + 3] = 255;
		}

		ThreadPool->Finished[Index] = true;
	}
}

// TOOD: See here to understand: https://wiki.unrealengine.com/Dynamic_Textures
void UpdateTextureRegions(UTexture2D* Texture, int32 MipIndex, uint32_t NumRegions, FUpdateTextureRegion2D* Regions, uint32_t SrcPitch, uint32_t SrcBpp, uint8* SrcData, bool bFreeData)
{
	if (Texture && Texture->Resource)
	{
		struct FUpdateTextureRegionsData
		{
			FTexture2DResource* Texture2DResource;
			int32 MipIndex;
			uint32_t NumRegions;
			FUpdateTextureRegion2D* Regions;
			uint32_t SrcPitch;
			uint32_t SrcBpp;
			uint8* SrcData;
		};

		FUpdateTextureRegionsData* RegionData = new FUpdateTextureRegionsData;

		RegionData->Texture2DResource = (FTexture2DResource*)Texture->Resource;
		RegionData->MipIndex = MipIndex;
		RegionData->NumRegions = NumRegions;
		RegionData->Regions = Regions;
		RegionData->SrcPitch = SrcPitch;
		RegionData->SrcBpp = SrcBpp;
		RegionData->SrcData = SrcData;

		ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
			UpdateTextureRegionsData,
			FUpdateTextureRegionsData*, RegionData, RegionData,
			bool, bFreeData, bFreeData,
			{
				for (uint32_t RegionIndex = 0; RegionIndex < RegionData->NumRegions; ++RegionIndex)
				{
					int32 CurrentFirstMip = RegionData->Texture2DResource->GetCurrentFirstMip();
					if (RegionData->MipIndex >= CurrentFirstMip)
					{
						RHIUpdateTexture2D(
							RegionData->Texture2DResource->GetTexture2DRHI(),
							RegionData->MipIndex - CurrentFirstMip,
							RegionData->Regions[RegionIndex],
							RegionData->SrcPitch,
							RegionData->SrcData
							+ RegionData->Regions[RegionIndex].SrcY * RegionData->SrcPitch
							+ RegionData->Regions[RegionIndex].SrcX * RegionData->SrcBpp
							);
					}
				}
				if (bFreeData)
				{
					FMemory::Free(RegionData->Regions);
					FMemory::Free(RegionData->SrcData);
				}
				delete RegionData;
			});
	}
}

// The all-important thread pool
static ProceduralWorkerThreads *WorkerThreads = 0;

// We're getting pretty messy here...
static size_t InstanceCount = 0;

AProceduralStaticMeshActor::AProceduralStaticMeshActor() {
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	InstanceCount++;
}

AProceduralStaticMeshActor::~AProceduralStaticMeshActor() {
	
	delete DynamicColours;
	delete UpdateTextureRegion;

	// REALLY want to make this cleaner, but it might not happen
	InstanceCount--;
	if (InstanceCount == 0) {
		delete WorkerThreads;
	}
}

void AProceduralStaticMeshActor::PostInitializeComponents() {
	Super::PostInitializeComponents();

	TextureBounds = FVector(0, 64, 64);

	FVector Origin(0, 0, 0);
	GetActorBounds(false, Origin, this->Bounds);
	Bounds = TextureBounds;

	// Convert the static material in our mesh into a dynamic one, and store it (please note that if you have more than one material that you wish to mark dynamic, do so here).
	DynamicMat = GetStaticMeshComponent()->CreateAndSetMaterialInstanceDynamic(0);
	
	// Create a dynamic texture with the default compression (B8G8R8A8)
	DynamicTexture = UTexture2D::CreateTransient(Bounds.Y, Bounds.Z);
	
	// Make sure it won't be compressed
	DynamicTexture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
	
	// Turn off Gamma-correction
	DynamicTexture->SRGB = 0;
	
	// Guarantee no garbage collection by adding it as a root reference
	DynamicTexture->AddToRoot();
	
	// Update the texture with new variable values.
	DynamicTexture->UpdateResource();
		
	// Set the Paramater in our material to our texture
	DynamicMat->SetTextureParameterValue("Texture2DParam", DynamicTexture);
	UpdateTextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, Bounds.Y, Bounds.Z);

	DataSize = Bounds.Y * Bounds.Z * 4;
	DataSizeSqrt = Bounds.Y * 4;

	DynamicColours = new uint8[DataSize];

	// Y is the texture width and Z is the height
	uint32_t NumPixels = (uint32_t)(Bounds.Z * Bounds.Y);
	uint32_t ChunkSize = NumPixels / NUM_THREADS;
	uint32_t LastChunkSize = ChunkSize + (NumPixels % NUM_THREADS);

	// If there's some left over at the end (or Bounds.Z * Bounds.Y < NUM_THREADS), the last thread will get that;
	// otherwise, every thread gets the same size chunk
	if (LastChunkSize)
	{
		for (int i = 0; i < NUM_THREADS - 1; i++) 
		{
			ChunkOffsets[i] = i * ChunkSize;
			ChunkSizes[i] = ChunkSize;
		}
		ChunkOffsets[NUM_THREADS - 1] = NumPixels - LastChunkSize;
		ChunkSizes[NUM_THREADS - 1] = LastChunkSize;
	}
	else
	{
		for (int i = 0; i < NUM_THREADS; i++)
		{
			ChunkOffsets[i] = i * Bounds.Y * ChunkSize;
			ChunkSizes[i] = ChunkSize;
		}
	}

	if (WorkerThreads == NULL) {
		WorkerThreads = new ProceduralWorkerThreads();
	}
}

void AProceduralStaticMeshActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	ElapsedTime += DeltaTime;

	// Since the ThreadPool's Data will change between invocations, this is necessary to do each time
	//uint32_t Size = Bounds.Z * Bounds.Y;
	//for (uint32_t i = 0; i < Size; i++) {
	//	uint32_t Y = i / (uint32_t)Bounds.Y;
	//	uint32_t Z = i % (uint32_t)Bounds.Y;

	//	double ShiftedY = (Y / Bounds.Y * 2) - 1.0;
	//	double ShiftedZ = (Z / Bounds.Z * 2) - 1.0;

	//	// Use the lookup tables, which are hopefully faster than computing the values
	//	float mov0 = ShiftedY + ShiftedZ + cos(sin(ElapsedTime) * 2.0) * 100.f + sin(ShiftedY / 100.f) * 1000.f;
	//	float mov1 = ShiftedY / 0.9 + ElapsedTime;
	//	float mov2 = ShiftedY / 0.2;
	//	float c1 = fabs(sin(mov1 + ElapsedTime) / 2.f + mov2 / 2.f - mov1 - mov2 + ElapsedTime);
	//	float c2 = fabs(sin(c1 + sin(mov0 / 1000.f + ElapsedTime) + sin(ShiftedZ / 40.f + ElapsedTime) + sin((ShiftedY + ShiftedZ) / 100.f) * 3.f));
	//	float c3 = fabs(sin(c2 + cos(mov1 + mov2 + c2) + cos(mov2) + sin(ShiftedY / 1000.f)));

	//	// TODO: Need some check here to make sure we don't write to the buffer if the thread stopped
	//	// *Theoretically*, nothing should be freed before we finish the loop, but I don't really trust that. At all.
	//	DynamicColours[i * 4] = (uint8)(c1 * 255);
	//	DynamicColours[(i * 4) + 1] = (uint8)(c2 * 255);
	//	DynamicColours[(i * 4) + 2] = (uint8)(c3 * 255);
	//	DynamicColours[(i * 4) + 3] = 255;
	//}

	WorkerThreads->DoProceduralGeneration(DynamicColours, ChunkOffsets, ChunkSizes, &TextureBounds, ElapsedTime);

	UpdateTextureRegions(DynamicTexture, 0, 1, UpdateTextureRegion, DataSizeSqrt, (uint32_t)4, DynamicColours, false);
	DynamicMat->SetTextureParameterValue("Texture2DParam", DynamicTexture);
}