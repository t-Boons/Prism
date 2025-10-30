// Copyright tBoons 2025

#include "PrismLensFlareAsset.h"

#if WITH_EDITOR
void UPrismLensFlareAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	OnAssetChanged.Broadcast();
}

void UPrismLensFlareAsset::BeginDestroy()
{
	Super::BeginDestroy();
	OnAssetBeingDeleted.Broadcast();
}
void UPrismLensFlareAsset::PostRename(UObject* OldOuter, const FName OldName)
{
	Super::PostRename(OldOuter, OldName);
	OnAssetRenamed.Broadcast();
}
#endif