// Copyright tBoons 2025

#pragma once

#include "CoreMinimal.h"
#include "PrismLensFlareAssetData.h"

class FRDGBuilder;
struct FScreenPassTexture;
class UTexture;
struct FPrismScreenLight;

class PRISM_API FPrismRenderer
{
public:
    /// <summary>
    /// Clears the currently rendered Prism lens flare asset.
    /// </summary>
    void Clear();

    /// <summary>
	/// Set the lens flare bokeh assets to be used on the screen.
    /// </summary>
    void SetPrismLensFlareAsset(const TArray<FPrismLensFlareAssetData>& InPrismAsset);

    /// <summary>
	/// Execute render pass used to render the lens-flares on the screen.
    /// </summary>
    void Render_RenderThread(FRDGBuilder& GraphBuilder, const FScreenPassTexture& SceneColor, const TArray<FPrismScreenLight>& Lights);

private:

    /// <summary>
    /// Bokeh data the renderer uses to render the lens flares.
    /// </summary>
    struct PRISM_API FBokehGPUData
    {
        uint32 TextureIndex;
        float AxisOffsetMultiplier;
        FVector2f Scale;
        FLinearColor Color;
        float Rotation;
        float RingEffectContribution;
        float RingEffectSize;
        float MiddleFadeContribution;
        float MiddleFadeSize;
        int32 AlwaysLookAtLight;
    };

private:
    // We use a TStrongObjectPtr to make sure the GC does not free these textures while they are used by the renderer.
    TArray<TStrongObjectPtr<UTexture>> BokehUTextures;
    TArray<FBokehGPUData> BokehGPUData;
};