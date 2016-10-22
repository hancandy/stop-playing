#include "StopPlaying.h"
#include "ControlPanelActorConnector.h"

#include "ControlPanel.h"

void UControlPanelActorConnector::OnInteract(APawn* InteractingPawn)
{
    Super::OnInteract(InteractingPawn);

    AControlPanel* ParentControlPanel = Cast<AControlPanel>(GetOwner());

    if(!ParentControlPanel) { return; }
    
    ParentControlPanel->SetConnectedActor(ConnectedActor);
}
