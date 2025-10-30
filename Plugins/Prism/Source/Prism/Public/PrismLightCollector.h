// Copyright tBoons 2025

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "HAL/CriticalSection.h" 
#include "Containers/Map.h"
#include "PrismScreenLight.h"

class ADirectionalLight;
class FSceneView;

class PRISM_API FPrismLightCollector
{
public:
    void EnqueLightDataRequest_GameThread(const FSceneView& InView);
    TArray<FPrismScreenLight> GetLightDataRequest_RenderThread(const FSceneView& InView);
    void ClearLightDataRequests();

private:
    TMap<uint32, TArray<FPrismScreenLight>> LightQueryMap;
    FCriticalSection Mutex;
};