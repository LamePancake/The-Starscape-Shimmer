// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "TrigLookup.h"
#include "ProceduralStaticMeshActor.h"
#include "AllowWindowsPlatformTypes.h"
#include <atomic>
#include <cmath>
#include "HideWindowsPlatformTypes.h"

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

AProceduralStaticMeshActor::AProceduralStaticMeshActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

AProceduralStaticMeshActor::~AProceduralStaticMeshActor() {
	
	WorkerThreads->Release();
	delete DynamicColours;
	delete UpdateTextureRegion;
}

void AProceduralStaticMeshActor::PostInitializeComponents() {
	Super::PostInitializeComponents();
	WorkerThreads = ProceduralThreadPool::GetInstance();

	TextureBounds = FVector(0, 64, 64);

	//FTransform CurrentTransform = ActorToWorld();

	FVector Origin(0, 0, 0);
	GetActorBounds(false, Origin, this->Bounds);
	Bounds = TextureBounds;///= 3;

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
}

void AProceduralStaticMeshActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	ElapsedTime += DeltaTime;

	WorkerThreads->DoGeneration(DynamicColours, &TextureBounds, ElapsedTime, CirclePlasma);

	UpdateTextureRegions(DynamicTexture, 0, 1, UpdateTextureRegion, DataSizeSqrt, (uint32_t)4, DynamicColours, false);
	DynamicMat->SetTextureParameterValue("Texture2DParam", DynamicTexture);
}