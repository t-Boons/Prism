// Copyright tBoons 2025

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"
#include "PrismLensFlareAsset.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

/// <summary>
/// FAssetTypeActions_PrismAsset is used to register the PrismLensFlareAsset type and make it available in the editor when right clicking to create a new asset.
/// </summary>
class PRISMEDITOR_API FAssetTypeActions_PrismAsset : public FAssetTypeActions_Base
{
public:
    virtual FText GetName() const override { return NSLOCTEXT("Prism LensFlare Asset", "AssetTypeActions_Prism", "Prism LensFlare Asset"); }
    virtual FColor GetTypeColor() const override { return FColor(255, 200, 50); }
    virtual UClass* GetSupportedClass() const override { return UPrismLensFlareAsset::StaticClass(); }
    virtual uint32 GetCategories() override { return EAssetTypeCategories::Misc; }
    virtual void GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder) override;
};