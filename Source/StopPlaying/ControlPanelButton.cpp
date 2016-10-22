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

    return ParentControlPanel->IsEffectActive(EffectType, EffectScale);
}

// --------------------
// Timer functions
// --------------------
void UControlPanelButton::StartTimer()
{
    ResetTimer();

    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UControlPanelButton::TickTimer, 1.f, true);

    Timer = Timeout;
}

void UControlPanelButton::TickTimer()
{
    Timer--;

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
}
