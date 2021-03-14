// Fill out your copyright notice in the Description page of Project Settings.


#include "WebCamReader.h"

#include "Rendering/Texture2DResource.h"

// Sets default values
AWebCamReader::AWebCamReader()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraID = 0;
	RefreshRate = 15;
	bIsStreamOpen = false;
	VideoSize = FVector2D(0, 0);
	ShouldResize = false;
	ResizeDimensions = FVector2D(320, 240);
	RefreshTimer = 0.0f;
	Stream = cv::VideoCapture();
	Frame = cv::Mat();
}

// Called when the game starts or when spawned
void AWebCamReader::BeginPlay()
{
	Super::BeginPlay();

	Stream.open(CameraID);
	UE_LOG(LogTemp, Log, TEXT("begin"));

	if (Stream.isOpened())
	{
		UE_LOG(LogTemp, Log, TEXT("open"));

		bIsStreamOpen = true;
		UpdateFrame();
		VideoSize = FVector2D(Frame.cols, Frame.rows);
		Size = cv::Size(ResizeDimensions.X, ResizeDimensions.Y);
		VideoTexture = UTexture2D::CreateTransient(VideoSize.X, VideoSize.Y);
		VideoTexture->UpdateResource();
		VideoUpdateTextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, VideoSize.X, VideoSize.Y);

		Data.Init(FColor(0, 0, 0, 255), VideoSize.X * VideoSize.Y);

		DoProcessing();
		UpdateTexture();
		OnNextVideoFrame();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("webcam open failed"));
	}
}

// Called every frame
void AWebCamReader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// UE_LOG(LogTemp, Log, TEXT("update frame"));

	RefreshTimer += DeltaTime;
	if (bIsStreamOpen && RefreshTimer >= 1.0f / RefreshRate)
	{
		RefreshTimer -= 1.0f / RefreshRate;
		UpdateFrame();
		DoProcessing();
		UpdateTexture();
		OnNextVideoFrame();
	}
}

void AWebCamReader::UpdateFrame()
{
	if (Stream.isOpened())
	{
		Stream.read(Frame);

		if (ShouldResize)
		{
			cv::resize(Frame, Frame, Size);
		}
	}
	else
	{
		bIsStreamOpen = false;
	}
}

void AWebCamReader::DoProcessing()
{
}

void AWebCamReader::UpdateTexture()
{
	if (bIsStreamOpen && Frame.data)
	{
		// UE_LOG(LogTemp, Log, TEXT("update frame"));
		// 
		// Copy Mat data to Data array
		for (int y = 0; y < VideoSize.Y; y++)
		{
			for (int x = 0; x < VideoSize.X; x++)
			{
				int i = x + (y * VideoSize.X);
				Data[i].B = Frame.data[i * 3 + 0];
				Data[i].G = Frame.data[i * 3 + 1];
				Data[i].R = Frame.data[i * 3 + 2];
			}
		}

		UpdateTextureRegions(
			VideoTexture,
			static_cast<int32>(0),
			static_cast<uint32>(1),
			VideoUpdateTextureRegion,
			static_cast<uint32>(4 * VideoSize.X),
			static_cast<uint32>(4),
			reinterpret_cast<uint8*>(Data.GetData()),
			false
		);
	}
}

void AWebCamReader::UpdateTextureRegions(UTexture2D* Texture, int32 MipIndex, uint32 NumRegions,
                                         FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp,
                                         uint8* SrcData, bool bFreeData) const
{
	if (Texture->Resource)
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

		RegionData->Texture2DResource = static_cast<FTexture2DResource*>(Texture->Resource);
		RegionData->MipIndex = MipIndex;
		RegionData->NumRegions = NumRegions;
		RegionData->Regions = Regions;
		RegionData->SrcPitch = SrcPitch;
		RegionData->SrcBpp = SrcBpp;
		RegionData->SrcData = SrcData;

		ENQUEUE_RENDER_COMMAND(UpdateTextureRegionsData)(
			[RegionData, bFreeData](FRHICommandListImmediate& RHICmdList)
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
