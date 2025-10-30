// Copyright tBoons 2025

#include "AssetTypeActions_PrismAsset.h"
#include "PrismAssetSubsystem.h"
#include "EditorStyleSet.h"

void FAssetTypeActions_PrismAsset::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder)
{
	TArray<TWeakObjectPtr<UPrismLensFlareAsset>> Assets = GetTypedWeakObjectPtrs<UPrismLensFlareAsset>(InObjects);

	// Early out if the asset array is empty.
	if(Assets.IsEmpty())
	{
		return;
	}

	// Get the frist element.
	TWeakObjectPtr<UPrismLensFlareAsset> FirstAsset = Assets[0];

	// Add Menu Entry button when right-clicking the asset.
	MenuBuilder.AddMenuEntry(
		NSLOCTEXT("AssetTypeActions", "Prism_DoSomething", "Use this Asset"),
		NSLOCTEXT("AssetTypeActions", "Prism_DoSomethingTooltip", "Sets the currently selected Prism asset to the one used."),
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Play"),
		FUIAction(
			FExecuteAction::CreateLambda([FirstAsset]()
				{
					UPrismAssetSubSystem* SubSystem = GEngine->GetEngineSubsystem<UPrismAssetSubSystem>();
					if (SubSystem)
					{
						SubSystem->SetGlobalPrismLensFlareAssetAndSaveToConfig(FirstAsset.Get());
					}
				})));
}