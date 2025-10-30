// Copyright tBoons 2025

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "PrismAssetFactory.generated.h"

/// <summary>
/// PrismAssetfactory is required to create a new PrismLensFlareAsset in the editor.
/// </summary>
UCLASS()
class PRISMEDITOR_API UPrismAssetFactory : public UFactory
{
	GENERATED_BODY()

public:
	UPrismAssetFactory();

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* Parent, FName Name, EObjectFlags Flags, UObject* Object, FFeedbackContext* FeedbackContext) override;
};