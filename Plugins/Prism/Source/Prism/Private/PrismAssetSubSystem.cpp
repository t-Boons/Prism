// Copyright tBoons 2025

#include "PrismAssetSubSystem.h"
#include "Prism.h"
#include "PrismLensFlareAsset.h"
#include "PrismViewExtention.h"
#include "PrismRendererSubSystem.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "HAL/FileManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Engine/Engine.h"

void UPrismAssetSubSystem::SetGlobalPrismLensFlareAsset(UPrismLensFlareAsset* NewFlare)
{
    GlobalPrismPlayMode = NewFlare;

    UPrismRendererSubSystem* SubSystem = GEngine->GetEngineSubsystem<UPrismRendererSubSystem>();
    if (SubSystem)
    {
        if (NewFlare)
        {
            SubSystem->SetLensFlareBokehs(NewFlare->Bokehs);
        }
        else
        {
            SubSystem->ClearLensFlareBokehs();
        }
    }
}

void UPrismAssetSubSystem::SetGlobalPrismLensFlareAssetAndSaveToConfig(UPrismLensFlareAsset* NewFlare)
{
    // Clear bindings of the previous asset.
    if (GlobalPrism)
    {
        GlobalPrism->OnAssetChanged.Clear();
        GlobalPrism->OnAssetBeingDeleted.Clear();
        GlobalPrism->OnAssetRenamed.Clear();
    }

    SetGlobalPrismLensFlareAsset(NewFlare);

    if (NewFlare)
    {
		GlobalPrism = NewFlare;
        GlobalPrismAssetPath = NewFlare->GetPathName();
        SaveGlobalPrismLensFlareConfig();

        // Add the new bindings.
        GlobalPrism->OnAssetChanged.AddLambda([this, NewFlare]() {SetGlobalPrismLensFlareAsset(NewFlare); });
        GlobalPrism->OnAssetBeingDeleted.AddLambda([this]() {SetGlobalPrismLensFlareAssetAndSaveToConfig(nullptr); });

        // We update the asset path in the config when the asset is renamed so it doesnt get invalidated upon restarting the engine.
        GlobalPrism->OnAssetRenamed.AddLambda([this]()
            {
                if (GlobalPrism)
                {
                    GlobalPrismAssetPath = GlobalPrism->GetPathName();
                    SaveGlobalPrismLensFlareConfig();
                }
            });
    }
}

UPrismLensFlareAsset* UPrismAssetSubSystem::GetGlobalPrismLensFlareAsset() const
{
    return GlobalPrismPlayMode;
}

void UPrismAssetSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
    // This will initialize the UPrismRendererSubSystem since this subsystem depends on it.
    Collection.InitializeDependency(UPrismRendererSubSystem::StaticClass());
    LoadGlobalPrismLensFlareFromConfig();
    

    // We use this Delegate to switch to a changeable global prism in playmode. So that we can set it back to the original when leaving playmode.
    OnPostWorldInitializationDelegate = FWorldDelegates::OnPostWorldInitialization.AddLambda([this](UWorld* World, const UWorld::InitializationValues IVS)
        {
            if (World && World->IsPlayInEditor())
            {
				GlobalPrismPlayMode = GlobalPrism;
            }
		});

    OnWorldCleanupDelegate = FWorldDelegates::OnWorldCleanup.AddLambda([this](UWorld* World, bool bSessionEnded, bool bCleanupResources)
        {
            if (World && World->IsPlayInEditor())
            {
				SetGlobalPrismLensFlareAsset(GlobalPrism);
            }
        });
}

void UPrismAssetSubSystem::Deinitialize()
{
    // Clean up and unbind the delegates that were added during Initialization.
    if (OnPostWorldInitializationDelegate.IsValid())
    {
        FWorldDelegates::OnPostWorldInitialization.Remove(OnPostWorldInitializationDelegate);
    }

    if (OnWorldCleanupDelegate.IsValid())
    {
		FWorldDelegates::OnWorldCleanup.Remove(OnWorldCleanupDelegate);
    }
}

FString GetConfigDir()
{
    const FPrismModule& Module = FModuleManager::LoadModuleChecked<FPrismModule>(TEXT("Prism"));
    const FString PluginConfigDir = FPaths::Combine(Module.PluginDirectory(), TEXT("Config"));
    return PluginConfigDir;
}

FString GetIniPath()
{
    return FConfigCacheIni::NormalizeConfigIniPath(GetConfigDir() / TEXT("PrismConfig.ini"));
}

void CreateConfigFileIfNotExist(const FString& IniPath)
{
    if (!FPaths::FileExists(IniPath))
    {
        IFileManager::Get().MakeDirectory(*GetConfigDir(), true);
        FFileHelper::SaveStringToFile(TEXT(""), *IniPath);
    }
}

bool DoesAssetExist(const FString& AssetPath)
{
    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

    FAssetData AssetData = AssetRegistry.GetAssetByObjectPath(FSoftObjectPath(AssetPath));
    return AssetData.IsValid();
}

void UPrismAssetSubSystem::SaveGlobalPrismLensFlareConfig()
{
    const FString IniPath = GetIniPath();

    CreateConfigFileIfNotExist(IniPath);

    const FString SectionName = TEXT("/Script/Prism.UPrismAssetSubSystem");
    const FString KeyName = TEXT("GlobalAssetPath");

    if (FConfigSection* Section = ConfigFile.Find(SectionName))
    {
        Section->Remove(*KeyName);
    }

    ConfigFile.SetString(*SectionName, *KeyName, *GlobalPrismAssetPath);
    ConfigFile.Write(IniPath);
}

void UPrismAssetSubSystem::LoadGlobalPrismLensFlareFromConfig()
{
    const FString IniPath = GetIniPath();
    CreateConfigFileIfNotExist(IniPath);

    ConfigFile.Read(IniPath);
    ConfigFile.GetString(TEXT("/Script/Prism.UPrismAssetSubSystem"), TEXT("GlobalAssetPath"), GlobalPrismAssetPath);

    if (DoesAssetExist(GlobalPrismAssetPath))
    {
        // Load the asset and set it.
		TSoftObjectPtr<UPrismLensFlareAsset> LoadedAssetPtr(GlobalPrismAssetPath);
        SetGlobalPrismLensFlareAssetAndSaveToConfig(LoadedAssetPtr.LoadSynchronous());
    }
    else
    {
		GlobalPrismAssetPath = TEXT("");
        UE_LOG(LogTemp, Warning, TEXT("Saved asset reference is invalid or empty: %s"), *GlobalPrismAssetPath);
    }
}
