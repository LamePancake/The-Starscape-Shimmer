// Fill out your copyright notice in the Description page of Project Settings.

#include "TheStarscapeShimmer.h"
#include "ProceduralStaticMeshActor.h"

// TOOD: See here to understand: https://wiki.unrealengine.com/Dynamic_Textures
void UpdateTextureRegions(UTexture2D* Texture, int32 MipIndex, uint32 NumRegions, FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData)
{
	if (Texture && Texture->Resource)
	{
		struct FUpdateTextureRegionsData
		{
			FTexture2DResource* Texture2DResource;
			int32 MipIndex;
			uint32 NumRegions;
			FUpdateTextureRegion2D* Regions;
			uint32 SrcPitch;
			uint32 SrcBpp;
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
				for (uint32 RegionIndex = 0; RegionIndex < RegionData->NumRegions; ++RegionIndex)
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

AProceduralStaticMeshActor::AProceduralStaticMeshActor() {
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AProceduralStaticMeshActor::PostInitializeComponents() {
	Super::PostInitializeComponents();

	FVector Origin(0, 0, 0);
	GetActorBounds(false, Origin, this->Bounds);

	// Convert the static material in our mesh into a dynamic one, and store it (please note that if you have more than one material that you wish to mark dynamic, do so here).
	DynamicMat = GetStaticMeshComponent()->CreateAndSetMaterialInstanceDynamic(0);
	
	// Create a dynamic texture with the default compression (B8G8R8A8)
	DynamicTexture = UTexture2D::CreateTransient(Bounds.X, Bounds.Y);
	
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
	UpdateTextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, Bounds.X, Bounds.Y);

	DataSize = Bounds.X * Bounds.Y * 4;
	DataSizeSqrt = Bounds.X * 4;

	DynamicColours = new uint8[DataSize];
}

void AProceduralStaticMeshActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	ElapsedTime += DeltaTime;

	uint32 Size = DataSize / 4;
	for (uint32 i = 0; i < Size; i++) {
		uint32 Y = i / (uint32)Bounds.X;
		uint32 X = i % (uint32)Bounds.X;

		double ShiftedX = (X / Bounds.X * 2) - 1.0;
		double ShiftedY = (Y / Bounds.Y * 2) - 1.0;

		float mov0 = ShiftedX + ShiftedY + cos(sin(ElapsedTime) * 2.0) * 100.f + sin(ShiftedX / 100.f) * 1000.f;
		float mov1 = ShiftedX / 0.9 + ElapsedTime;
		float mov2 = ShiftedX / 0.2;
		float c1 = fabs(sin(mov1 + ElapsedTime) / 2.f + mov2 / 2.f - mov1 - mov2 + ElapsedTime);
		float c2 = fabs(sin(c1 + sin(mov0 / 1000.f + ElapsedTime) + sin(ShiftedY / 40.f + ElapsedTime) + sin((ShiftedX + ShiftedY) / 100.f) * 3.f));
		float c3 = fabs(sin(c2 + cos(mov1 + mov2 + c2) + cos(mov2) + sin(ShiftedX / 1000.f)));

		DynamicColours[i * 4] = (uint8)(c1 * 255);
		DynamicColours[(i * 4) + 1] = (uint8)(c2 * 255);
		DynamicColours[(i * 4) + 2] = (uint8)(c3 * 255);
		DynamicColours[(i * 4) + 3] = 255;
	}
	UpdateTextureRegions(DynamicTexture, 0, 1, UpdateTextureRegion, DataSizeSqrt, (uint32)4, DynamicColours, false);
	DynamicMat->SetTextureParameterValue("Texture2DParam", DynamicTexture);
}