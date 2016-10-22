#include "StopPlaying.h"
#include "ControlPanelActorConnector.h"

#include "ControlPanel.h"

void UControlPanelActorConnector::OnInteract(APawn* InteractingPawn)
{
    Super::OnInteract(InteractingPawn);

    AControlPanel* ParentControlPanel = Cast<AControlPanel>(GetOwner());

    if(!ParentControlPanel) { return; }
    
    ParentControlPanel->SetConnectedActor(ConnectedActor, InitialTransform);
}

void UControlPanelActorConnector::Init()
{
    Super::Init();

    if(!ConnectedActor) { return; }

    InitialTransform = ConnectedActor->GetTransform();   

    SetLabel(ConnectedActor->GetName());
}
