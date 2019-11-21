#include "SkiaUE4.h"
#include "Engine/Texture2D.h"
#include "Modules/ModuleManager.h"
#include "include/core/SkGraphics.h"
#include "include/effects/SkGradientShader.h"

class FSkiaModule : public IModuleInterface
{
public:
	virtual void StartupModule() override
	{
		SkGraphics::Init();
	}

	virtual void ShutdownModule() override
	{
		SkGraphics::Term();
	}
};

USkSurface *USkSurface::MakeRaster(UObject *WorldContextObject, const FSkImageInfo &InImageInfo)
{
	USkSurface &Surface = *NewObject<USkSurface>(WorldContextObject);
	Surface.ImageInfo = std::move(InImageInfo);
	auto &II = Surface.ImageInfo.Inner;
	auto &Tb = Surface.TextureBuffer;

	Surface.Surface = SkSurface::MakeRaster(II);
	Tb.Size = II.width() * II.height() * II.bytesPerPixel();

	Tb.Texture = UTexture2D::CreateTransient(II.width(), II.height());
	Tb.Texture->SRGB = 0;

	Tb.Data.SetNum(Tb.Size);

	Surface.Canvas = NewObject<USkCanvas>(WorldContextObject);
	Surface.Canvas->SetOwner(&Surface);
	return &Surface;
}

void USkSurface::UpdateTexture()
{
	if (TextureBuffer.Texture == nullptr)
	{
		return;
	}

	FTexture2DMipMap &Mip = TextureBuffer.Texture->PlatformData->Mips[0];
	void *Data = Mip.BulkData.Lock(LOCK_READ_WRITE);
	Surface->getCanvas()->readPixels(ImageInfo.Inner, Data, ImageInfo.Inner.width() * ImageInfo.Inner.bytesPerPixel(),
	                                 0, 0);

	//FMemory::Memcpy(Data, TextureBuffer.Data.GetData(), TextureBuffer.Size);
	Mip.BulkData.Unlock();
	TextureBuffer.Texture->UpdateResource();
}

FSkImageInfo USkia::MakeSkImageInfo(int Width, int Height, ESkColorType ColorType, ESkAlphaType AlphaType)
{
	FSkImageInfo ImageInfo;
	ImageInfo.Inner = SkImageInfo::Make(Width, Height, (SkColorType)ColorType, (SkAlphaType)AlphaType);
	return ImageInfo;
}

FSkRect USkia::MakeSkRectXYWH(float X, float Y, float W, float H)
{
	return { SkRect::MakeXYWH(X, Y, W, H) };
}

FSkShader USkia::MakeLinearSkGradientShader(const FVector2D &Start, const FVector2D &End, const TArray<FColor> &Colors,
                                            const TArray<float> &Pos, ESkTileMode TileMode)
{
	TArray<SkColor> SkColors;
	for (auto &Color : Colors)
	{
		SkColors.Add(SkColorSetARGB(Color.A, Color.R, Color.G, Color.B));
	}

	const SkPoint pts[2] = { { Start.X, Start.Y }, { End.X, End.Y } };
	return { SkGradientShader::MakeLinear(pts, SkColors.GetData(), Pos.Num() == 0 ? nullptr : Pos.GetData(),
		                                  SkColors.Num(), (SkTileMode)TileMode) };
}

USkottieAnimation *USkottieAnimation::Make(UObject *WorldContextObject, const FString &JsonString)
{
	skottie::Animation::Builder Builder;
	USkottieAnimation &Animation = *NewObject<USkottieAnimation>(WorldContextObject);
	Animation.Animation = Builder.make(TCHAR_TO_ANSI(*JsonString), JsonString.Len());
	return &Animation;
}

IMPLEMENT_MODULE(FSkiaModule, SkiaUE4)
