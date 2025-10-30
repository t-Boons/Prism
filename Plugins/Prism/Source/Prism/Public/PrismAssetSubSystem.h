// Copyright tBoons 2025

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "Misc/ConfigCacheIni.h"

#include "PrismAssetSubSystem.generated.h"

class UPrismLensFlareAsset;
class FPrismViewExtention;
class FPrismModule;

/// <summary>
/// FPrismSubSystem is mainly used to store the GlobalLensFlareAsset the engine uses. Along with loading and saving it to a config file.
/// </summary>
/// 
UCLASS()
class PRISM_API UPrismAssetSubSystem : public UEngineSubsystem
{
    GENERATED_BODY()

public:
    /// <summary>
    /// Initialize the subsystem.
    /// </summary>
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    /// <summary>
    /// DeInitialize the subsystem.
    /// </summary>
    virtual void Deinitialize() override;

    /// <summary>
	/// Save the currently set GlobalPrismLensFlareAsset to config file.
    /// </summary>
    void SaveGlobalPrismLensFlareConfig();

    /// <summary>
	/// Load the GlobalPrismLensFlareAsset from config file.
    /// </summary>
    void LoadGlobalPrismLensFlareFromConfig();


    /// <summary>
	/// Set the global prinsm lens flare asset (also used by blueprint).
    /// </summary>
    UFUNCTION(BlueprintCallable, Category = "Prism")
    void SetGlobalPrismLensFlareAsset(UPrismLensFlareAsset* NewFlare);

    void SetGlobalPrismLensFlareAssetAndSaveToConfig(UPrismLensFlareAsset* NewFlare);

    /// <summary>
	/// Get the currently used global prism lens flare asset (also used by blueprint).
    /// </summary>
    UFUNCTION(BlueprintPure, Category = "Prism")
    UPrismLensFlareAsset* GetGlobalPrismLensFlareAsset() const;


private:
    TObjectPtr<UPrismLensFlareAsset> GlobalPrism;
    TObjectPtr<UPrismLensFlareAsset> GlobalPrismPlayMode;
	FString GlobalPrismAssetPath;
	FDelegateHandle OnPostWorldInitializationDelegate;
    FDelegateHandle OnWorldCleanupDelegate;
    FConfigFile ConfigFile;
};