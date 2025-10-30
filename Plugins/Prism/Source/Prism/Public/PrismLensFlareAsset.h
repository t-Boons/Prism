// Copyright tBoons 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PrismLensFlareAssetData.h"
#include "PrismLensFlareAsset.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAssetChanged);
DECLARE_MULTICAST_DELEGATE(FOnDeleting);
DECLARE_MULTICAST_DELEGATE(FOnAssetRenamed);
/// <summary>
/// UPrismLensFlareAsset is used to store an array of lens flare bokeh data as an asset.
/// </summary>
UCLASS(BlueprintType)
class PRISM_API UPrismLensFlareAsset : public UObject
{
	GENERATED_BODY()

public:

#if WITH_EDITOR
	/// <summary>
	/// Broadcasts the OnAssetChanged event.
	/// </summary>
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	/// <summary>
	/// Broadcasts the OnAssetBeingDeleted event.
	/// </summary>
	virtual void BeginDestroy() override;

	/// <summary>
	/// Called when asset is renamed.
	/// </summary>
	virtual void PostRename(UObject* OldOuter, const FName OldName) override;
#endif

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PrismAsset")
	TArray<FPrismLensFlareAssetData> Bokehs;

	FOnAssetRenamed OnAssetRenamed;
	FOnAssetChanged OnAssetChanged;
	FOnDeleting OnAssetBeingDeleted;
};
