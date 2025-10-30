// Copyright tBoons 2025

#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "PrismLensFlareAssetData.generated.h"

/// <summary>
/// FPrismLensFlareAssetData is the container for individual lens flare element data used in UPrismLensFlareAsset
/// </summary>
USTRUCT(BlueprintType)
struct PRISM_API FPrismLensFlareAssetData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prism")
	TObjectPtr<UTexture2D> Texture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prism")
	FLinearColor Color = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prism", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float Opacity = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prism")
	FVector2D Scale = { 1.0f, 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prism")
	float AxisOffset = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prism")
	float Rotation = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Prism")
	bool bEnableRingEffect = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prism", meta = (ClampMin = "0.0", ClampMax = "1.0", EditCondition = "bEnableRingEffect", EditConditionsHide))
	float RingEffectContribution = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prism", meta = (ClampMin = "0.0", ClampMax = "1.0", EditCondition = "bEnableRingEffect", EditConditionHides))
	float RingEffectSize = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Prism")
	bool bEnableMiddleFade = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prism", meta = (ClampMin = "0.0", ClampMax = "1.0", EditCondition = "bEnableMiddleFade", EditConditionsHide))
	float MiddleFadeContribution = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prism", meta = (ClampMin = "0.0", ClampMax = "1.0", EditCondition = "bEnableMiddleFade", EditConditionHides))
	float MiddleFadeSize = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prism")
	bool bAlwaysFaceLight = false;
};