// Copyright tBoons 2025

#include "PrismLightCollector.h"
#include "Engine/DirectionalLight.h"
#include "Engine/PointLight.h"
#include "EngineUtils.h"
#include "Components/LightComponent.h"
#include "SceneView.h"
#include "GameFramework/PlayerController.h"
#include "SceneInterface.h"

bool WorldToScreenUV(const FVector& ViewLocation, const FVector& ViewForward, const FIntRect& Resolution, const FMatrix& VP, const FVector& WorldPosition, FVector2D& Out)
{
    FVector2D ScreenPos;
    FSceneView::ProjectWorldToScreen(WorldPosition, Resolution, VP, ScreenPos, true);

    const FVector ToPoint = (WorldPosition - ViewLocation).GetSafeNormal();
    const float Dot = FVector::DotProduct(ViewForward, ToPoint);
    if (Dot < 0.0f)
    {
        return false;
    }
    Out = ScreenPos / static_cast<FVector2D>(Resolution.Size());
    return true;
}

void FPrismLightCollector::EnqueLightDataRequest_GameThread(const FSceneView& InView)
{
	check(IsInGameThread())
    const uint64 ViewKey = InView.GetViewKey();

    // We use NoAAMatrix to make sure the matrix does not Jitter due to AA.
    const FMatrix ProjMatrix = InView.ViewMatrices.ComputeProjectionNoAAMatrix();
    const FMatrix ViewMatrix = InView.ViewMatrices.GetViewMatrix();
    const FMatrix ViewProjectionMatrix = ViewMatrix * ProjMatrix;
    const UWorld* World = InView.Family->Scene->GetWorld();

    FCollisionQueryParams Params(SCENE_QUERY_STAT(SunOcclusion), true);

    // Try to ignore the player pawn if there is one.
	if (APlayerController* PlayerController = World->GetFirstPlayerController())
	{
		if (APawn* Pawn = PlayerController->GetPawnOrSpectator())
		{
			Params.AddIgnoredActor(Pawn);
		}
	}

	TArray<FPrismScreenLight> LightDataArray;

	// Get the light's direction and check if it's occluded.
	for (TActorIterator<ADirectionalLight> It(World); It; ++It)
	{
		const ADirectionalLight* LightActor = *It;
		const FVector LightDir = LightActor->GetActorForwardVector();
		const FVector LightLocation = -LightDir * 10000000.0f;

		FHitResult Hit;
		bool bBlocked = World->LineTraceSingleByChannel(
			Hit,
			InView.ViewLocation,
			LightLocation,
			ECC_Visibility,
			Params
		);

		FVector2D ScreenPos;
		if (!WorldToScreenUV(InView.ViewLocation, InView.ViewRotation.Vector(), InView.UnconstrainedViewRect, ViewProjectionMatrix, LightLocation, ScreenPos))
		{
			bBlocked = true;
		}

		if (!bBlocked)
		{
			FPrismScreenLight LightData;
			LightData.ScreenPosition = ScreenPos;
			LightData.LightColor = LightActor->GetLightColor();
			LightDataArray.Add(LightData);
		}
	}

	Mutex.Lock();
	LightQueryMap.Add(ViewKey, LightDataArray);
	Mutex.Unlock();
}

TArray<FPrismScreenLight> FPrismLightCollector::GetLightDataRequest_RenderThread(const FSceneView& InView)
{
	check(IsInRenderingThread());

    const uint64 ViewKey = InView.GetViewKey();

	FScopeLock Lock(&Mutex);
    if(!LightQueryMap.Contains(ViewKey))
	{
		return TArray<FPrismScreenLight>();
	}

	return *LightQueryMap.Find(ViewKey);
}

void FPrismLightCollector::ClearLightDataRequests()
{
	Mutex.Lock();
	LightQueryMap.Empty();
	Mutex.Unlock();
}
