#include "StopPlaying.h"
#include "ControlPanelButton.h"

#include "ControlPanel.h"

void UControlPanelButton::OnInteract(APawn* InteractingPawn)
{
    Super::OnInteract(InteractingPawn);

    ToggleEffect();
}

FString UControlPanelButton::GetEffectTypeString()
{
    switch(EffectType)
    {
        case EControlPanelEffectType::GRAVITY:
            return "Gravity";

        case EControlPanelEffectType::COLLISION:
            return "Collision";
        
        case EControlPanelEffectType::ROTATION:
            return "Rotation";
        
        case EControlPanelEffectType::TRANSLATION:
            return "Translation";
        
        case EControlPanelEffectType::SCALE:    
            return "Scale";
        
        case EControlPanelEffectType::WORLD_TIME:
            return "Time";
        
        case EControlPanelEffectType::WORLD_GRAVITY:
            return "Gravity";
        
    }

    return "(None)";
}

void UControlPanelButton::SetAppropriateLabel()
{
    bool bIsEffectActive = IsEffectActive();
    FString NewLabel = GetEffectTypeString();

    if(bIsEffectActive)
    {
        NewLabel += ": ON";
    }
    else
    {
        NewLabel += ": OFF";
    }
    
    SetLabel(NewLabel);
}

// --------------------
// Effect functions
// --------------------
void UControlPanelButton::ToggleEffect()
{
    SetEffectActive(!IsEffectActive());
}

void UControlPanelButton::SetEffectActive(bool bIsEffectActive)
{
    AControlPanel* ParentControlPanel = Cast<AControlPanel>(GetOwner());

    if(!ParentControlPanel) { return; }

    ParentControlPanel->SetEffectActive(EffectType, EffectScale, bIsEffectActive);
}

bool UControlPanelButton::IsEffectActive()
{
    AControlPanel* ParentControlPanel = Cast<AControlPanel>(GetOwner());

    if(!ParentControlPanel) { return false; }

    return ParentControlPanel->IsEffectActive(EffectType);
}
