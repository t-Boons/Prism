// Copyright tBoons 2025

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/// <summary>
/// PrismEditorModule is used to register the PrismLensFlareAsset asset type in the editor.
/// </summary>
class PRISMEDITOR_API FPrismEditorModule : public IModuleInterface
{
public:

    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    TSharedPtr<class FAssetTypeActions_PrismAsset> AssetTypeActions;
};