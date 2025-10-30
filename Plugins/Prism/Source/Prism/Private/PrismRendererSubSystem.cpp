// Copyright tBoons 2025

#include "PrismRendererSubSystem.h"
#include "CoreMinimal.h"
#include "PrismViewExtention.h"
#include "PrismRenderer.h"
#include "PrismLightCollector.h"

void UPrismRendererSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Renderer = MakeShared<FPrismRenderer>();
	LightCollector = MakeShared<FPrismLightCollector>();
	SceneViewExtension = FSceneViewExtensions::NewExtension<FPrismViewExtention>(*Renderer, *LightCollector);
}

void UPrismRendererSubSystem::Deinitialize()
{
	SceneViewExtension.Reset();
	Renderer.Reset();
	LightCollector.Reset();
}

void UPrismRendererSubSystem::SetLensFlareBokehs(const TArray<FPrismLensFlareAssetData>& Bokehs)
{
	Renderer->SetPrismLensFlareAsset(Bokehs);
}

void UPrismRendererSubSystem::ClearLensFlareBokehs()
{
	Renderer->Clear();
}