// Copyright tBoons 2025

#include "PrismRenderer.h"
#include "RenderGraphFwd.h"
#include "RenderGraphUtils.h"
#include "SceneView.h"
#include "ShaderParameterStruct.h"
#include "ScreenPass.h"
#include "PrismScreenLight.h"
#include "Engine/Texture.h"
#include "CommonRenderResources.h"
#include "Shader.h"
#include "RenderGraphBuilder.h"
#include "TextureResource.h"

class FPrismRendererPS : public FGlobalShader
{
public:
    DECLARE_EXPORTED_SHADER_TYPE(FPrismRendererPS, Global, );

    SHADER_USE_PARAMETER_STRUCT(FPrismRendererPS, FGlobalShader);

    BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
        SHADER_PARAMETER_SAMPLER(SamplerState, InputTextureSampler)
        SHADER_PARAMETER_RDG_BUFFER_SRV(StructuredBuffer<FBokehData>, BokehDataBuffer)
        SHADER_PARAMETER_RDG_TEXTURE(Texture2D<float4>, BokehTextures0)
        SHADER_PARAMETER_RDG_TEXTURE(Texture2D<float4>, BokehTextures1)
        SHADER_PARAMETER_RDG_TEXTURE(Texture2D<float4>, BokehTextures2)
        SHADER_PARAMETER_RDG_TEXTURE(Texture2D<float4>, BokehTextures3)
        SHADER_PARAMETER_RDG_TEXTURE(Texture2D<float4>, BokehTextures4)
        SHADER_PARAMETER_RDG_TEXTURE(Texture2D<float4>, BokehTextures5)
        SHADER_PARAMETER_RDG_TEXTURE(Texture2D<float4>, BokehTextures6)
        SHADER_PARAMETER_RDG_TEXTURE(Texture2D<float4>, BokehTextures7)
        SHADER_PARAMETER_RDG_TEXTURE(Texture2D<float4>, BokehTextures8)
        SHADER_PARAMETER_RDG_TEXTURE(Texture2D<float4>, BokehTextures9)
        SHADER_PARAMETER_RDG_TEXTURE(Texture2D<float4>, BokehTextures10)
        SHADER_PARAMETER_RDG_TEXTURE(Texture2D<float4>, BokehTextures11)
        SHADER_PARAMETER_RDG_TEXTURE(Texture2D<float4>, BokehTextures12)
        SHADER_PARAMETER_RDG_TEXTURE(Texture2D<float4>, BokehTextures13)
        SHADER_PARAMETER_RDG_TEXTURE(Texture2D<float4>, BokehTextures14)
        SHADER_PARAMETER_RDG_TEXTURE(Texture2D<float4>, BokehTextures15)
        SHADER_PARAMETER(FVector4f, LightColor)
        SHADER_PARAMETER(uint32, NumBokehData)
        RENDER_TARGET_BINDING_SLOTS()
    END_SHADER_PARAMETER_STRUCT()
};

class FPrismRendererVS : public FGlobalShader
{
public:
    DECLARE_EXPORTED_SHADER_TYPE(FPrismRendererVS, Global, );

    SHADER_USE_PARAMETER_STRUCT(FPrismRendererVS, FGlobalShader);
    BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
        SHADER_PARAMETER(float, AspectRatio)
        SHADER_PARAMETER_RDG_BUFFER_SRV(StructuredBuffer<FBokehData>, BokehDataBuffer)
        SHADER_PARAMETER(uint32, NumBokehData)
        SHADER_PARAMETER(FVector2f, LightScreenPosition)
    END_SHADER_PARAMETER_STRUCT()
};


BEGIN_SHADER_PARAMETER_STRUCT(FPRismRendererShaderParameters, )
    SHADER_PARAMETER_STRUCT(FPrismRendererVS::FParameters, VS)
    SHADER_PARAMETER_STRUCT(FPrismRendererPS::FParameters, PS)
END_SHADER_PARAMETER_STRUCT()


IMPLEMENT_SHADER_TYPE(, FPrismRendererVS, TEXT("/Plugins/Prism/Private/PrismRendererShader.usf"), TEXT("MainVS"), SF_Vertex);
IMPLEMENT_SHADER_TYPE(, FPrismRendererPS, TEXT("/Plugins/Prism/Private/PrismRendererShader.usf"), TEXT("MainPS"), SF_Pixel);

void FPrismRenderer::Clear()
{
    BokehUTextures.Empty();
    BokehGPUData.Empty();
}

void FPrismRenderer::SetPrismLensFlareAsset(const TArray<FPrismLensFlareAssetData>& InPrismAsset)
{
    const uint32 Size = InPrismAsset.Num();

    Clear();

    BokehGPUData.SetNum(InPrismAsset.Num());

    // Iterate over all Prism bokeh assets and populate the GPU data structure.
    for (size_t i = 0; i < InPrismAsset.Num(); i++)
    {
        FBokehGPUData& Bokeh = BokehGPUData[i];
        const FPrismLensFlareAssetData& AssetBokeh = InPrismAsset[i];

        Bokeh.Color = AssetBokeh.Color;
		Bokeh.Color.A = FMath::Clamp(AssetBokeh.Opacity, 0.0f, 1.0f);
        Bokeh.AxisOffsetMultiplier = AssetBokeh.AxisOffset;
        Bokeh.Scale = FVector2f(AssetBokeh.Scale);
		Bokeh.Rotation = FMath::DegreesToRadians(AssetBokeh.Rotation);

        const float RingEffectContribution = FMath::Clamp(AssetBokeh.RingEffectContribution, 0.0f, 1.0f);
        Bokeh.RingEffectContribution = AssetBokeh.bEnableRingEffect ? RingEffectContribution : 0.0f;

		Bokeh.RingEffectSize = AssetBokeh.RingEffectSize;
		Bokeh.AlwaysLookAtLight = AssetBokeh.bAlwaysFaceLight ? 1 : 0;

        const float MiddleFadeContribution = FMath::Clamp(AssetBokeh.MiddleFadeContribution, 0.0f, 1.0f);
        Bokeh.MiddleFadeContribution = AssetBokeh.bEnableMiddleFade ? MiddleFadeContribution : 0.0f;

		Bokeh.MiddleFadeSize = AssetBokeh.MiddleFadeSize;

        bool bTextureFound = false;
        for (int32 j = 0; j < BokehUTextures.Num(); j++)
        {
            // Texture is not found on the asset. Defaulting to white texture at index 15.
            if (!AssetBokeh.Texture)
            {
                Bokeh.TextureIndex = 15;
                continue;
            }

            // If the texture is already in the array, use its index.
            if (BokehUTextures[j]->GetUniqueID() == AssetBokeh.Texture->GetUniqueID())
            {
                Bokeh.TextureIndex = j;
                bTextureFound = true;
            }
        }

        // If the texture is not found but the asset has a texture add it to the texture list. And use its index.
        if (!bTextureFound && AssetBokeh.Texture)
        {
            // Force the texture to be streamed in fully.
            AssetBokeh.Texture->SetForceMipLevelsToBeResident(30.0f);
			AssetBokeh.Texture->NeverStream = true;

            BokehUTextures.Add(TStrongObjectPtr(AssetBokeh.Texture.Get()));
            Bokeh.TextureIndex = BokehUTextures.Num() - 1;
        }
    }
}

inline void LoadPSParameterTextures(FRDGBuilder& Builder, FPrismRendererPS::FParameters& Parameters, const TArray<TStrongObjectPtr<UTexture>>& Textures)
{
    // Set up the bokeh textures to white as default. (Could not get texture arrays to work in time so this will do for now)
	FRDGTextureRef WhiteTexture = FScreenPassTexture::GetWhiteTexture(Builder).Texture;

    Parameters.BokehTextures0 = WhiteTexture;
    Parameters.BokehTextures1 = WhiteTexture;
    Parameters.BokehTextures2 = WhiteTexture;
    Parameters.BokehTextures3 = WhiteTexture;
    Parameters.BokehTextures4 = WhiteTexture;
    Parameters.BokehTextures5 = WhiteTexture;
    Parameters.BokehTextures6 = WhiteTexture;
    Parameters.BokehTextures7 = WhiteTexture;
    Parameters.BokehTextures8 = WhiteTexture;
    Parameters.BokehTextures9 = WhiteTexture;
    Parameters.BokehTextures10 = WhiteTexture;
    Parameters.BokehTextures11 = WhiteTexture;
    Parameters.BokehTextures12 = WhiteTexture;
    Parameters.BokehTextures13 = WhiteTexture;
    Parameters.BokehTextures14 = WhiteTexture;
    Parameters.BokehTextures15 = WhiteTexture;

    // Iterate over all BokehUTextures and set them to the right texture. Index is stored in the Bokeh Structured buffer.
    for (int32 i = 0; i < Textures.Num(); i++)
    {
        const TStrongObjectPtr<UTexture>& Tex = Textures[i];

        if (Tex && Tex->GetResource())
        {
            FRDGTextureRef Texture = Builder.RegisterExternalTexture(
                CreateRenderTarget(Tex->GetResource()->TextureRHI,
                    *FString::Printf(TEXT("PrismRenderTexture %d"), i)),
                *FString::Printf(TEXT("PrismRenderRDGTexture %d"), i));

            switch (i)
            {
            case 0: Parameters.BokehTextures0 = Texture; break;
            case 1: Parameters.BokehTextures1 = Texture; break;
            case 2: Parameters.BokehTextures2 = Texture; break;
            case 3: Parameters.BokehTextures3 = Texture; break;
            case 4: Parameters.BokehTextures4 = Texture; break;
            case 5: Parameters.BokehTextures5 = Texture; break;
            case 6: Parameters.BokehTextures6 = Texture; break;
            case 7: Parameters.BokehTextures7 = Texture; break;
            case 8: Parameters.BokehTextures8 = Texture; break;
            case 9: Parameters.BokehTextures9 = Texture; break;
            case 10: Parameters.BokehTextures10 = Texture; break;
            case 11: Parameters.BokehTextures11 = Texture; break;
            case 12: Parameters.BokehTextures12 = Texture; break;
            case 13: Parameters.BokehTextures13 = Texture; break;
            case 14: Parameters.BokehTextures14 = Texture; break;
            case 15: Parameters.BokehTextures15 = Texture; break;
            }
        }
    }
}

void FPrismRenderer::Render_RenderThread(FRDGBuilder& GraphBuilder, const FScreenPassTexture& SceneColor, const TArray<FPrismScreenLight>& Lights)
{
    check(IsInRenderingThread());

    if (Lights.IsEmpty())
    {
        return;
    }

	// TODO: Support multiple lights.
	const FPrismScreenLight& Light = Lights[0];

    // Create the SRV used to pass the bokeh data to the shader.
    FRDGBufferSRVRef Srv = GraphBuilder.CreateSRV(CreateStructuredBuffer(GraphBuilder, TEXT("PrismRendererBokehs"), this->BokehGPUData));

    // Set up vertex shader parameters.
    FPrismRendererVS::FParameters VSParameters;
    VSParameters.BokehDataBuffer = Srv;
    VSParameters.AspectRatio = static_cast<float>(SceneColor.ViewRect.Width()) / static_cast<float>(SceneColor.ViewRect.Height());
    VSParameters.NumBokehData = this->BokehGPUData.Num();
    VSParameters.LightScreenPosition = FVector2f(Light.ScreenPosition);



    // Set up the shader parameters for the pixel shader.
    FPrismRendererPS::FParameters PSParameters;
    PSParameters.InputTextureSampler = TStaticSamplerState<SF_Bilinear>::GetRHI();
    PSParameters.RenderTargets[0] = FRenderTargetBinding(SceneColor.Texture, ERenderTargetLoadAction::ELoad);
    PSParameters.BokehDataBuffer = Srv;
    PSParameters.NumBokehData = this->BokehGPUData.Num();
	PSParameters.LightColor = FVector4f(Lights[0].LightColor);

	LoadPSParameterTextures(GraphBuilder, PSParameters, this->BokehUTextures);

    // Conbine pixel and vertex shader parameters.
    FPRismRendererShaderParameters* Parameters = GraphBuilder.AllocParameters<FPRismRendererShaderParameters>();
    Parameters->VS = VSParameters;
    Parameters->PS = PSParameters;

    // Add the lens flare pass to the render graph.
    GraphBuilder.AddPass(
        RDG_EVENT_NAME("PrismLensFlarePass"),
        Parameters,
        ERDGPassFlags::Raster,
        [this, Parameters, SceneColor](FRHICommandList& RHICmdList)
        {
            // Set the rendering viewport.
            RHICmdList.SetViewport(
                SceneColor.ViewRect.Min.X,
                SceneColor.ViewRect.Min.Y,
                0.0f,
                SceneColor.ViewRect.Max.X,
                SceneColor.ViewRect.Max.Y,
                1.0f);


            // Initialize the PSO.
            FGraphicsPipelineStateInitializer GraphicsPSOInit;
            RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);
            GraphicsPSOInit.BlendState = TStaticBlendState<CW_RGBA, BO_Add, BF_SourceAlpha, BF_InverseSourceAlpha, BO_Add, BF_One, BF_Zero>::GetRHI();
            GraphicsPSOInit.RasterizerState = TStaticRasterizerState<>::GetRHI();
            GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI();
            static TShaderMapRef<FPrismRendererVS> VertexShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
            static TShaderMapRef<FPrismRendererPS> PixelShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
            GraphicsPSOInit.BoundShaderState.VertexShaderRHI = VertexShader.GetVertexShader();
            GraphicsPSOInit.BoundShaderState.PixelShaderRHI = PixelShader.GetPixelShader();
            GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = GFilterVertexDeclaration.VertexDeclarationRHI;
            GraphicsPSOInit.PrimitiveType = PT_TriangleList;
            SetGraphicsPipelineState(RHICmdList, GraphicsPSOInit, 0);

            // Set the shader parameters.
            SetShaderParameters(RHICmdList, VertexShader, VertexShader.GetVertexShader(), Parameters->VS);
            SetShaderParameters(RHICmdList, PixelShader, PixelShader.GetPixelShader(), Parameters->PS);

            // Submit instanced draw call based on how many bokehs there are.
            RHICmdList.SetStreamSource(0, nullptr, 0);
            RHICmdList.DrawPrimitive(0, 2, Parameters->VS.NumBokehData);
        });
}