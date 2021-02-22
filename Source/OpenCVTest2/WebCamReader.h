//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "opencv2/core.hpp"
//#include "opencv2/highgui.hpp"
//#include "opencv2/imgproc.hpp"
//#include "opencv2/videoio.hpp"
//
//#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
//#include "WebCamReader.generated.h"
//
//UCLASS()
//class OPENCVTEST2_API AWebCamReader final : public AActor
//{
//	GENERATED_BODY()
//	
//	public:	
//	// Sets default values for this actor's properties
//	AWebCamReader();
//
//	protected:
//	// Called when the game starts or when spawned
//	virtual void BeginPlay() override;
//
//	public:	
//	// Called every frame
//	virtual void Tick(float DeltaTime) override;
// 
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=WebCam)
//	int32 CameraID;
// 
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=WebCam)
//	bool ShouldResize;
// 
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=WebCam)
//	FVector2D ResizeDimensions;
// 
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=WebCam)
//	float RefreshRate;
// 
//	UPROPERTY(BlueprintReadWrite, Category=WebCam)
//	float RefreshTimer;
// 
//	UFUNCTION(BlueprintImplementableEvent, Category=WebCam)
//    void OnNextVideoFrame();
// 
//	cv::Mat Frame;
//	cv::VideoCapture Stream;
//	cv::Size Size;
//
//	void UpdateFrame();
//	void DoProcessing();
//	void UpdateTexture();
//	
//	UPROPERTY(BlueprintReadOnly, Category=WebCam)
//	bool bIsStreamOpen;
//	
//	UPROPERTY(BlueprintReadWrite, Category=WebCam)
//	FVector2D VideoSize;
//	
//	UPROPERTY(BlueprintReadOnly, Category=WebCam)
//	UTexture2D* VideoTexture;
//	
//	UPROPERTY(BlueprintReadOnly, Category=WebCam)
//	TArray<FColor> Data;
//	
//	protected:
//	void UpdateTextureRegions(UTexture2D* Texture, int32 MipIndex, uint32 NumRegions, FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData) const;
//	
//	FUpdateTextureRegion2D* VideoUpdateTextureRegion;
//};
