// Copyright tBoons 2025

#include "Prism.h"
#include "ShaderCore.h"
#include "Interfaces/IPluginManager.h"

void FPrismModule::StartupModule()
{
	// Add the shader source directory mapping so the engine can find the shader files in the plugin.
    const FString PluginShaderDir = FPaths::Combine(PluginDirectory(), TEXT("Shaders"));
    AddShaderSourceDirectoryMapping(TEXT("/Plugins/Prism"), PluginShaderDir);
}

void FPrismModule::ShutdownModule()
{
}

FString FPrismModule::PluginDirectory() const
{
    return IPluginManager::Get().FindPlugin(TEXT("Prism"))->GetBaseDir();
}

IMPLEMENT_MODULE(FPrismModule, Prism)