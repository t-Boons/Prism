// Copyright tBoons 2025

#include "PrismAssetFactory.h"
#include "PrismLensFlareAsset.h"

UPrismAssetFactory::UPrismAssetFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UPrismLensFlareAsset::StaticClass();
}


UObject* UPrismAssetFactory::FactoryCreateNew(UClass* Class, UObject* Parent, FName Name, EObjectFlags Flags, UObject* Object, FFeedbackContext* FeedbackContext)
{
	return NewObject<UPrismLensFlareAsset>(Parent, Class, Name, Flags);
}