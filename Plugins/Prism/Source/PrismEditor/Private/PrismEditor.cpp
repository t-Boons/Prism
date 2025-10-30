// Copyright tBoons 2025

#include "PrismEditor.h"
#include "AssetTypeActions_PrismAsset.h"
#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "PrismEditor"

void FPrismEditorModule::StartupModule()
{
    IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
    AssetTypeActions = MakeShareable(new FAssetTypeActions_PrismAsset);
    AssetTools.RegisterAssetTypeActions(AssetTypeActions.ToSharedRef());
}

void FPrismEditorModule::ShutdownModule()
{
}

IMPLEMENT_MODULE(FPrismEditorModule, PrismEditor)

#undef LOCTEXT_NAMESPACE
