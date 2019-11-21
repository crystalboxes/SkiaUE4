#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkFont.h"
#include "include/core/SkPaint.h"
#include "include/core/SkRect.h"
#include "include/core/SkSurface.h"
#include "modules/skottie/include/Skottie.h"
//
#include "SkiaUE4.generated.h"

class UTexture2D;

class USkSurface;

UENUM(BlueprintType)
enum class ESkColorType : uint8
{
	kUnknown_SkColorType, //!< uninitialized
	kAlpha_8_SkColorType, //!< pixel with alpha in 8-bit byte
	kRGB_565_SkColorType, //!< pixel with 5 bits red, 6 bits green, 5 bits blue, in 16-bit word
	kARGB_4444_SkColorType, //!< pixel with 4 bits for alpha, red, green, blue; in 16-bit word
	kRGBA_8888_SkColorType, //!< pixel with 8 bits for red, green, blue, alpha; in 32-bit word
	kRGB_888x_SkColorType, //!< pixel with 8 bits each for red, green, blue; in 32-bit word
	kBGRA_8888_SkColorType, //!< pixel with 8 bits for blue, green, red, alpha; in 32-bit word
	kRGBA_1010102_SkColorType, //!< 10 bits for red, green, blue; 2 bits for alpha; in 32-bit word
	kRGB_101010x_SkColorType, //!< pixel with 10 bits each for red, green, blue; in 32-bit word
	kGray_8_SkColorType, //!< pixel with grayscale level in 8-bit byte
	kRGBA_F16Norm_SkColorType, //!< pixel with half floats in [0,1] for red, green, blue, alpha; in 64-bit word
	kRGBA_F16_SkColorType, //!< pixel with half floats for red, green, blue, alpha; in 64-bit word
	kRGBA_F32_SkColorType, //!< pixel using C float for red, green, blue, alpha; in 128-bit word
};

UENUM(BlueprintType)
enum class ESkAlphaType : uint8
{
	kUnknown_SkAlphaType, //!< uninitialized
	kOpaque_SkAlphaType, //!< pixel is opaque
	kPremul_SkAlphaType, //!< pixel components are premultiplied by alpha
	kUnpremul_SkAlphaType, //!< pixel components are independent of alpha
	kLastEnum_SkAlphaType, //!< last valid value
};

UENUM(BlueprintType)
enum class ESkTileMode : uint8
{
	kClamp,
	kRepeat,
	kMirror,
	kDecal,
	kLastTileMode = kDecal,
};

USTRUCT(BlueprintType)
struct FSkImageInfo
{
	GENERATED_BODY()

	SkImageInfo Inner;
};


UCLASS()
class SKIAUE4_API USkImageInfoImpl : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
		
	UFUNCTION(BlueprintCallable, Category = "Skia|ImageInfo")
	static int Width(const FSkImageInfo &ImageInfo)
	{
		return ImageInfo.Inner.width();
	}
	UFUNCTION(BlueprintCallable, Category = "Skia|ImageInfo")
	static int Height(const FSkImageInfo &ImageInfo)
	{
		return ImageInfo.Inner.height();
	}

	UFUNCTION(BlueprintCallable, Category = "Skia|ImageInfo")
	static int BytesPerPixel(const FSkImageInfo &ImageInfo)
	{
		return ImageInfo.Inner.bytesPerPixel();
	}

};

USTRUCT(BlueprintType)
struct FSkRect
{
	GENERATED_BODY()

	SkRect Inner;
};

USTRUCT(BlueprintType)
struct FSkShader
{
	GENERATED_BODY()

	sk_sp<SkShader> Inner = nullptr;
};

USTRUCT(BlueprintType)
struct FSkPaint
{
	GENERATED_BODY()
public:
	SkPaint Inner;
};

UCLASS()
class SKIAUE4_API USkPaintImpl : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "Skia|Paint")
	static void SetColor(UPARAM(ref) FSkPaint &Paint, const FColor &Color)
	{
		Paint.Inner.setColor(SkColorSetARGB(Color.A, Color.R, Color.G, Color.B));
	}

	UFUNCTION(BlueprintCallable, Category = "Skia|Paint")
	static void SetAntiAlias(UPARAM(ref) FSkPaint &Paint, bool Value)
	{
		Paint.Inner.setAntiAlias(Value);
	}

	UFUNCTION(BlueprintCallable, Category = "Skia|Paint")
	static void SetShader(UPARAM(ref) FSkPaint &Paint, const FSkShader &Value)
	{
		Paint.Inner.setShader(Value.Inner);
	}
};

USTRUCT(BlueprintType)
struct FSkFont
{
	GENERATED_BODY()
public:
	SkFont Inner;
};

UCLASS()
class SKIAUE4_API USkFontImpl : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "Skia|Font")
	static void SetSubpixel(UPARAM(ref) FSkFont &Font, bool Value)
	{
		Font.Inner.setSubpixel(Value);
	}

	UFUNCTION(BlueprintCallable, Category = "Skia|Font")
	static void SetSize(UPARAM(ref) FSkFont &Font, float Size)
	{
		Font.Inner.setSize(Size);
	}
};

UCLASS()
class SKIAUE4_API USkia : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure, Category = "Skia|ImageInfo")
	static FSkImageInfo MakeSkImageInfo(int Width, int Height, ESkColorType ColorType, ESkAlphaType AlphaType);

	UFUNCTION(BlueprintPure, Category = "Skia|Rect")
	static FSkRect MakeSkRectXYWH(float X, float Y, float W, float H);

	UFUNCTION(BlueprintPure, Category = "Skia|Shader")
	static FSkShader MakeLinearSkGradientShader(const FVector2D &Start, const FVector2D &End,
	                                            const TArray<FColor> &Colors, const TArray<float> &Pos,
	                                            ESkTileMode TileMode);
};

USTRUCT(BlueprintType)
struct FTextureBuffer
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	UTexture2D *Texture = nullptr;

	UPROPERTY(VisibleAnywhere)
	unsigned int Size = 0;

	TArray<uint8_t> Data;
};

UCLASS(BlueprintType)
class SKIAUE4_API USkSurface : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "Skia|Surface")
	UTexture2D *GetTexture()
	{
		return TextureBuffer.Texture;
	}
	UPROPERTY(VisibleAnywhere)
	FTextureBuffer TextureBuffer;

	UPROPERTY(VisibleAnywhere)
	USkCanvas *Canvas = nullptr;

	UPROPERTY(VisibleAnywhere)
	FSkImageInfo ImageInfo;

	UFUNCTION(BlueprintCallable, Category = "Skia|Surface", meta = (WorldContext = "WorldContextObject"))
	static USkSurface *MakeRaster(UObject *WorldContextObject, const FSkImageInfo &InImageInfo);

	UFUNCTION(BlueprintCallable, Category = "Skia|Surface", meta = (WorldContext = "WorldContextObject"))
	void UpdateTexture();

	FORCEINLINE sk_sp<SkSurface> GetSk()
	{
		return Surface;
	}

private:
	sk_sp<SkSurface> Surface = nullptr;
};

UCLASS(BlueprintType)
class SKIAUE4_API USkCanvas : public UObject
{
	GENERATED_BODY()
public:
	FORCEINLINE SkCanvas *GetSk()
	{
		return Owner->GetSk()->getCanvas();
	}

	UFUNCTION(BlueprintCallable, Category = "Skia|Canvas")
	void Clear(const FColor &Color)
	{
		GetSk()->clear(SkColorSetARGB(Color.A, Color.R, Color.G, Color.B));
	}

	UFUNCTION(BlueprintCallable, Category = "Skia|Canvas")
	void Save()
	{
		GetSk()->save();
	}

	UFUNCTION(BlueprintCallable, Category = "Skia|Canvas")
	void Restore()
	{
		GetSk()->restore();
	}

	UFUNCTION(BlueprintCallable, Category = "Skia|Canvas")
	void Translate(float Dx, float Dy)
	{
		GetSk()->translate(Dx, Dy);
	}

	UFUNCTION(BlueprintCallable, Category = "Skia|Canvas")
	void Rotate(float Degrees)
	{
		GetSk()->rotate(Degrees);
	}

	UFUNCTION(BlueprintCallable, Category = "Skia|Canvas")
	void DrawSimpleText(const FString &Text, const FVector2D &Start, const FSkFont &Font, const FSkPaint &Paint)
	{

		GetSk()->drawSimpleText(TCHAR_TO_UTF8(*Text), Text.Len(), SkTextEncoding::kUTF8, Start.X, Start.Y, Font.Inner,
		                        Paint.Inner);
	}

	UFUNCTION(BlueprintCallable, Category = "Skia|Canvas")
	void DrawRect(UPARAM(ref) FSkRect &Rect, const FSkPaint &Paint)
	{
		GetSk()->drawRect(Rect.Inner, Paint.Inner);
	}

	UFUNCTION(BlueprintCallable, Category = "Skia|Canvas")
	void DrawCircle(const FVector2D &Center, float Radius, const FSkPaint &Paint)
	{
		GetSk()->drawCircle(Center.X, Center.Y, Radius, Paint.Inner);
	}

	FORCEINLINE void SetOwner(USkSurface *InOwner)
	{
		Owner = InOwner;
	}

private:
	USkSurface *Owner = nullptr;
};

UCLASS(BlueprintType)
class SKIAUE4_API USkottieAnimation : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Skia|Skottie", meta = (WorldContext = "WorldContextObject"))
	static USkottieAnimation *Make(UObject *WorldContextObject, const FString &JsonString);

	// Returns the animation duration in seconds.
	UFUNCTION(BlueprintPure, Category = "Skia|Skottie")
	float Duration()
	{
		return Animation->duration();
	}

	// Returns the animation frame rate (frames / second).
	UFUNCTION(BlueprintPure, Category = "Skia|Skottie")
	float Fps()
	{
		return Animation->fps();
	}

	// Updates the animation state for |t|. (normalized [0..1])
	UFUNCTION(BlueprintCallable, Category = "Skia|Skottie")
	void Seek(float NormalizedTime)
	{
		Animation->seek(NormalizedTime);
	}

	// Update the animation state to match t, specifed in frame time
	// i.e. relative to duration().
	UFUNCTION(BlueprintCallable, Category = "Skia|Skottie")
	void SeekFrameTime(float Time)
	{
		Animation->seekFrameTime(Time);
	}

	// Draws the current animation frame.
	UFUNCTION(BlueprintCallable, Category = "Skia|Skottie")
	void Render(USkCanvas *Canvas, const FSkRect &DstR)
	{
		Animation->render(Canvas->GetSk(), &DstR.Inner);
	}

	FString Version()
	{
		return Animation->version().c_str();
	}

	FVector2D size() const
	{
		auto &s = Animation->size();
		return { s.width(), s.height() };
	}

private:
	sk_sp<skottie::Animation> Animation = nullptr;
};
