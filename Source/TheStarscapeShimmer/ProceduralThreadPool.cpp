// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "ProceduralThreadPool.h"

static uint32 ReferenceCount = 0;
static ProceduralThreadPool* Instance = nullptr;

///<description>
///Runs the code to generate textures given a function to use and a procedural thread pool object.
///</description>
class FProceduralRunnable : public FRunnable {

public:
	FProceduralRunnable(const int Index, ProceduralThreadPool* Threads)
		: Index(Index), ThreadPool(Threads)
	{}

	// We've already done all necessary initialisation in the constructor
	virtual bool Init() override {
		IsDone = false;
		return true;
	}

	virtual uint32 Run() override {
		while (!IsDone) {
			while (ThreadPool->Finished[Index]); // Wait until we actually have new data to process

			// Since the ThreadPool's data will change between invocations, this is necessary to do each time
			uint8* Data = ThreadPool->GetData();
			uint32_t Offset = ThreadPool->GetChunkOffset(Index);
			uint8* Chunk = Data + (Offset * 4);							// Each pixel is 4 bytes, so we have to skip ahead by offset * 4
			size_t Size = ThreadPool->GetChunkSize(Index);
			FVector Bounds = *(ThreadPool->GetBounds());				// Make a copy in case we're supposed to stop later
			ProceduralFunc Function = ThreadPool->GetProceduralFunc();
			double ElapsedTime = ThreadPool->GetElapsedTime();

			// Sanity check (actually just a hack to get around some weird threading problem, but "sanity check" sounds better)
			if (Bounds.Y < 1 || Bounds.Z < 1)
			{
				UE_LOG(LogTemp, Warning, TEXT("Invalid bounds: Bounds.Y = %lf, Bounds.Z = %lf"), Bounds.Y, Bounds.Z);
				break;
			}

			Vec2 Pos;
			Vec2 ShaderBounds = { Bounds.Y, Bounds.Z };
			for (uint32_t i = 0; i < Size; i++) {
				uint32_t Y = (i + Offset) / (uint32_t)Bounds.Y;
				uint32_t Z = (i + Offset) % (uint32_t)Bounds.Y;

				Pos.X = Y;
				Pos.Y = Z;

				Function(Chunk + i * 4, &Pos, &ShaderBounds, ElapsedTime);
			}

			ThreadPool->Finished[Index] = true;
		}
		return 0;
	}

	virtual void Exit() override {
		// Lol don't do anything here
	}

	virtual void Stop() override {
		IsDone = true;
		// Lol don't do anything here
	}

private:
	int Index;
	ProceduralThreadPool* ThreadPool;
	bool IsDone;
};

ProceduralThreadPool::ProceduralThreadPool(uint32 ThreadCount):
	NumThreads(ThreadCount), CompletionCounter(0), ShouldGenerate(false), ElapsedTime(0.0),
	ChunkOffsets(new uint32[ThreadCount]), ChunkSizes(new size_t[ThreadCount]), Finished(new std::atomic<bool>[ThreadCount])
{
	Threads = new FRunnableThread*[ThreadCount];
	Runnables = new FProceduralRunnable*[ThreadCount];

	// Initialise the new threads and set the 
	for (uint32 i = 0; i < ThreadCount; i++) {
		FString ThreadName = FString::Printf(TEXT("ProcThread %d"), i);
		Finished[i] = true;
		Runnables[i] = new FProceduralRunnable(i, this);
		Threads[i] = FRunnableThread::Create(Runnables[i], ThreadName.GetCharArray().GetData(), 0, TPri_Normal);
	}
}
ProceduralThreadPool::~ProceduralThreadPool()
{
	for (uint32 i = 0; i < NumThreads; i++) {
		// Kill the thread and wait for it to die
		Finished[i] = false;
		Threads[i]->Kill(true);
		delete Runnables[i];
		delete Threads[i];
	}

	delete[] Finished;
	delete[] Threads;
	delete[] ChunkSizes;
	delete[] ChunkOffsets;

	Instance = nullptr;
}


// Generates the texture
void ProceduralThreadPool::DoGeneration(uint8* Data, FVector* Bounds, double ElapsedTime, ProceduralFunc Function) {

	// Y is the texture width and Z is the height
	uint32 NumPixels = (uint32)(Bounds->Z * Bounds->Y);
	uint32 ChunkSize = NumPixels / NumThreads;
	uint32 Remainder = NumPixels % NumThreads;

	// If there's some left over at the end (or Bounds.Z * Bounds.Y < NUM_THREADS), the last thread will get that;
	// otherwise, every thread gets the same size chunk
	if (Remainder)
	{
		for (uint32 i = 0; i < NumThreads - 1; i++)
		{
			ChunkOffsets[i] = i * ChunkSize;
			ChunkSizes[i] = ChunkSize;
		}
		ChunkOffsets[NumThreads - 1] = NumPixels - (ChunkSize + Remainder);
		ChunkSizes[NumThreads - 1] = ChunkSize + Remainder;
	}
	else
	{
		for (uint32 i = 0; i < NumThreads; i++)
		{
			ChunkOffsets[i] = i * Bounds->Y * ChunkSize;
			ChunkSizes[i] = ChunkSize;
		}
	}

	this->Data = Data;
	this->Bounds = Bounds;
	this->ElapsedTime = ElapsedTime;
	this->Function = Function;

	// Tell the threads to start
	for (uint32 i = 0; i < NumThreads; i++) {
		Finished[i] = false;
	}
	unsigned int Iterations = 0;
	bool AllFinished = false;

	for (uint32 i = 0; i < NumThreads; i++) {
		Finished[i] = false;
	}

	// Continue looping through finished array until every thread has finished
	while (!AllFinished && Iterations < 100000000) {
		AllFinished = true;
		for (uint32 i = 0; i < NumThreads; i++) {
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


ProceduralThreadPool* ProceduralThreadPool::GetInstance()
{
	// TODO: Add some logic here to determine the appropriate number of threads
	// For now we'll go with 3, since that seemed to work pretty well
	uint32 ThreadCount = 3;

	if (Instance == nullptr)
	{
		Instance = new ProceduralThreadPool(ThreadCount);
	}

	++ReferenceCount;
	return Instance;
}

void ProceduralThreadPool::Release()
{
	if (--ReferenceCount == 0)
	{
		delete Instance;
	}
}
