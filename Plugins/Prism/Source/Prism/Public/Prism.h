// Copyright tBoons 2025

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class PRISM_API FPrismModule : public IModuleInterface
{
public:

    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    /// <summary>
	/// Get the plugin directory path.
    /// </summary>
    /// <returns></returns>
    FString PluginDirectory() const;
};