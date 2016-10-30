#pragma once

#include "InteractiveActor.h"

#include "Engine/TriggerVolume.h"
#include "InteractiveActorKillVolume.generated.h"

UCLASS(Blueprintable)
class STOPPLAYING_API AInteractiveActorKillVolume : public AActor
{
	GENERATED_BODY()
	
public:
    void NotifyActorBeginOverlap(AActor* OtherActor);
	
	
};
