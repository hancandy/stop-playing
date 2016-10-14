#include "StopPlaying.h"
#include "ControlPanelWidget.h"

void UControlPanelWidget::Init(AActor* Connected)
{
    ConnectedActor = Connected;
    ChildActor = Cast<AInteractiveActor>(GetChildActor());
            
    if(!ChildActor) {
        UE_LOG(LogTemp, Error, TEXT("%s has no child actor"), *GetName());
        return;
    }
    
    ChildActor->OnInteraction.AddDynamic(this, &UControlPanelWidget::OnInteract);

    SetLabel();
}

void UControlPanelWidget::OnInteract(APawn* InteractingPawn)
{
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

void UControlPanelWidget::ToggleEffect()
{
    switch(Type)
    {
        case EControlPanelWidgetType::GRAVITY_BUTTON:
            SetGravity(!GetGravity());
            break;

        case EControlPanelWidgetType::COLLISION_BUTTON:
            SetCollision(!GetCollision());
            break;
    }

    SetLabel();
}

bool UControlPanelWidget::HasConnectedActor()
{
    if(!ConnectedActor)
    {
        UE_LOG(LogTemp, Error, TEXT("%s has no connected AActor"), *GetName());
        return false;
    }

    return true;
}

void UControlPanelWidget::SetLabel(FString Suffix)
{
    if(!ChildActor) { return; }

    UTextRenderComponent* TextRenderComponent = ChildActor->FindComponentByClass<UTextRenderComponent>();

    if(TextRenderComponent)
    {
        FString NewLabel = Label;
        bool bState = false;
    
        switch(Type)
        {
            case EControlPanelWidgetType::GRAVITY_BUTTON:
                bState = GetGravity();
                break;

            case EControlPanelWidgetType::COLLISION_BUTTON:
                bState = GetCollision();
                break;
        }

        if(bState)
        {
            NewLabel += " ON";
        }
        else
        {
            NewLabel += " OFF";
        }

        NewLabel += Suffix;

        TextRenderComponent->SetText(FText::FromString(NewLabel));
    }
}

UPrimitiveComponent* UControlPanelWidget::GetActorPrimitiveComponent()
{
    if(!HasConnectedActor()) { return nullptr; }

    UPrimitiveComponent* PrimitiveComponent = ConnectedActor->FindComponentByClass<UPrimitiveComponent>();

    if(!PrimitiveComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("%s has no UPrimitiveComponent"), *ConnectedActor->GetName());
    }

    return PrimitiveComponent;
}

// --------------------
// Timer functions
// --------------------
void UControlPanelWidget::StartTimer()
{
    ResetTimer();

    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UControlPanelWidget::TickTimer, 1.f, true);

    Timer = Timeout;
    
    SetLabel(" (" + FString::FromInt(Timer) + ")");
}

void UControlPanelWidget::TickTimer()
{
    Timer--;

    if(Timer <= 0)
    {
        ResetTimer();
        ToggleEffect();
    }
    else
    {
        SetLabel(" (" + FString::FromInt(Timer) + ")");
    }
}

void UControlPanelWidget::ResetTimer()
{
    GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

    Timer = 0;

    SetLabel();
}

// --------------------
// Widget functions
// --------------------
bool UControlPanelWidget::GetGravity()
{
    UPrimitiveComponent* PrimitiveComponent = GetActorPrimitiveComponent();
    
    if(!PrimitiveComponent) { return false; }

    return PrimitiveComponent->IsGravityEnabled();
}

void UControlPanelWidget::SetGravity(bool bIsEnabled)
{
    UPrimitiveComponent* PrimitiveComponent = GetActorPrimitiveComponent();
    
    if(!PrimitiveComponent) { return; }

    PrimitiveComponent->SetEnableGravity(bIsEnabled);
}

bool UControlPanelWidget::GetCollision()
{
    if(!ConnectedActor) { return false; }

    return ConnectedActor->GetActorEnableCollision();
}

void UControlPanelWidget::SetCollision(bool bIsEnabled)
{
    if(!ConnectedActor) { return; }

    ConnectedActor->SetActorEnableCollision(bIsEnabled);
}
