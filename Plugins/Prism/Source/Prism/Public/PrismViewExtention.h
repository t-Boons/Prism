// Copyright tBoons 2025

#pragma once

#include "CoreMinimal.h"
#include "SceneViewExtension.h"
#include "SceneView.h"

class UWorld;
class FAutoRegister;
class FPrismLightCollector;
class FPrismRenderer;

/// <summary>
/// FPrismViewExtention is used to hook into the rendering pipeline and add the prism lens flare rendering before the bloom pass.
/// </summary>
class PRISM_API FPrismViewExtention : public FSceneViewExtensionBase
{
public:
    FPrismViewExtention(const FAutoRegister& AutoReg, FPrismRenderer& Renderer, FPrismLightCollector& LightCollector);

private:
    virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) override;
    virtual void SubscribeToPostProcessingPass(EPostProcessingPass Pass, const FSceneView& InView, FPostProcessingPassDelegateArray& InOutPassCallbacks, bool bIsPassEnabled) override;

    FScreenPassTexture RenderBeforeBloom_RenderThread(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessMaterialInputs& InOutInputs);

    FPrismRenderer& Renderer;
	FPrismLightCollector& LightCollector;
};
