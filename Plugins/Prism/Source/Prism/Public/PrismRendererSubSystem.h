// Copyright tBoons 2025

#pragma once

#include "Subsystems/EngineSubsystem.h"
#include "PrismLensFlareAssetData.h"

#include "PrismRendererSubSystem.generated.h"

class FPrismViewExtention;
class FPrismRenderer;
class FPrismLightCollector;

/// <summary>
/// UPrismSubSystem is mainly used to store the GlobalLensFlareAsset the engine uses. Along with loading and saving it to a config file.
/// </summary>
UCLASS()
class PRISM_API UPrismRendererSubSystem : public UEngineSubsystem
{
    GENERATED_BODY()

public:
    /// <summary>
    /// Initialize the subsystems.
    /// </summary>
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    /// <summary>
    /// DeInitialize the subsystems.
    /// </summary>
    virtual void Deinitialize() override;

    /// <summary>
    /// Set the lens flare bokehs that are currently being rendered.
    /// </summary>
    void SetLensFlareBokehs(const TArray<FPrismLensFlareAssetData>& Bokehs);

	/// <summary>
	/// Clear the lens flare bokehs thar are currently being rendered.
	/// </summary>
	void ClearLensFlareBokehs();

private:
    TSharedPtr<FPrismViewExtention> SceneViewExtension;
    TSharedPtr<FPrismRenderer> Renderer;
    TSharedPtr<FPrismLightCollector> LightCollector;
};
