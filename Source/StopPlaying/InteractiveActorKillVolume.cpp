#include "StopPlaying.h"
#include "InteractiveActorKillVolume.h"

// Reset
void AInteractiveActorKillVolume::NotifyActorBeginOverlap(AActor* OtherActor)
{
    if(!OtherActor) { return; }

    AInteractiveActor* InteractiveActor = Cast<AInteractiveActor>(OtherActor);

    if(!InteractiveActor) { return; }

    InteractiveActor->BeginReset();
}
