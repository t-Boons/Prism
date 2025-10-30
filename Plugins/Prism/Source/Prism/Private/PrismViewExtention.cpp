// Copyright tBoons 2025

#include "PrismViewExtention.h"

#include "Engine/World.h"
#include "SceneInterface.h"
#include "PostProcess/PostProcessMaterialInputs.h"
#include "PrismRenderer.h"
#include "PrismLightCollector.h"
#include "PrismRendererSubSystem.h"
#include "PostProcess/PostProcessing.h"

FPrismViewExtention::FPrismViewExtention(const FAutoRegister& AutoReg, FPrismRenderer& InRenderer, FPrismLightCollector& InLightCollector)
	: FSceneViewExtensionBase(AutoReg), Renderer(InRenderer), LightCollector(InLightCollector)
{
}

void FPrismViewExtention::SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView)
{
	UPrismRendererSubSystem* SubSystem = GEngine->GetEngineSubsystem<UPrismRendererSubSystem>();
    if (SubSystem)
    {
        LightCollector.EnqueLightDataRequest_GameThread(InView);
    }
}

void FPrismViewExtention::SubscribeToPostProcessingPass(EPostProcessingPass Pass, const FSceneView& InView, FPostProcessingPassDelegateArray& InOutPassCallbacks, bool bIsPassEnabled)
{
    // MotionBlur pass is marked as BL_SceneColorBeforeBloom. So it runs before the Bloom pass.
    // The bloom pass contains the existing lens-flare pass hence we try to get the delegates for the MotionBlur pass.
    if (Pass == EPostProcessingPass::MotionBlur)
    {
        InOutPassCallbacks.Add(FPostProcessingPassDelegate::CreateRaw(this, &FPrismViewExtention::RenderBeforeBloom_RenderThread));
    }
}
    
FScreenPassTexture FPrismViewExtention::RenderBeforeBloom_RenderThread(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessMaterialInputs& InOutInputs)
{
	check(IsInRenderingThread());

	FScreenPassTexture SceneColor = InOutInputs.ReturnUntouchedSceneColorForPostProcessing(GraphBuilder);
	const TArray<FPrismScreenLight> Lights = LightCollector.GetLightDataRequest_RenderThread(View);
	Renderer.Render_RenderThread(GraphBuilder, SceneColor, Lights);

	return SceneColor;
}