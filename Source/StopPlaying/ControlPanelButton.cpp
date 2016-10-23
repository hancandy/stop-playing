#include "StopPlaying.h"
#include "ControlPanelButton.h"

#include "ControlPanel.h"

void UControlPanelButton::OnInteract(APawn* InteractingPawn)
{
    Super::OnInteract(InteractingPawn);

    ToggleEffect();

    // Handle timer
    if(Timer > 0)
    {
        ResetTimer();
    }
    else if(Timeout > 0)
    {
        StartTimer();
    }
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
        
        case EControlPanelEffectType::TIME:
            return "Time";
        
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
    
    if(Timer > 0)
    {
        NewLabel += " (" + FString::FromInt(Timer) + ")";
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

// --------------------
// Timer functions
// --------------------
void UControlPanelButton::StartTimer()
{
    ResetTimer();

    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UControlPanelButton::TickTimer, 1.f, true);

    Timer = Timeout;
    
    SetAppropriateLabel();
}

void UControlPanelButton::TickTimer()
{
    Timer--;

    SetAppropriateLabel();

    if(Timer <= 0)
    {
        ResetTimer();
        ToggleEffect();
    }
}

void UControlPanelButton::ResetTimer()
{
    GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

    Timer = 0;

    SetAppropriateLabel();
}
