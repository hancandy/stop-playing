#include "StopPlaying.h"
#include "ControlPanelWidget.h"

UControlPanelWidget::UControlPanelWidget()
{
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}

void UControlPanelWidget::Init(AActor* Connected)
{
    ConnectedActor = Connected;
    ChildActor = Cast<AInteractiveActor>(GetChildActor());
            
    if(!ConnectedActor) { return; }
    
    if(!ChildActor) {
        UE_LOG(LogTemp, Error, TEXT("%s has no child actor"), *GetName());
        return;
    }
    
    ChildActor->OnInteraction.AddDynamic(this, &UControlPanelWidget::OnInteract);

    InitialPosition = ConnectedActor->GetActorLocation();
    InitialDirection = ConnectedActor->GetActorForwardVector();
    InitialScale = ConnectedActor->GetActorRelativeScale3D();

    SetEffectActive(bInitialState);

    SetLabel();
}

void UControlPanelWidget::OnInteract(APawn* InteractingPawn)
{
    if(!bIsWidgetActive) { return; }

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
    SetEffectActive(!IsEffectActive());

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

        if(IsEffectActive())
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

UMeshComponent* UControlPanelWidget::GetActorMeshComponent()
{
    if(!HasConnectedActor()) { return nullptr; }

    UMeshComponent* MeshComponent = ConnectedActor->FindComponentByClass<UMeshComponent>();

    if(!MeshComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("%s has no UMeshComponent"), *ConnectedActor->GetName());
    }

    return MeshComponent;
}

void UControlPanelWidget::TickWidget(float DeltaTime)
{
    if(!bIsWidgetActive) { return; }
    if(!ConnectedActor) { return; }
    
    FVector NewVector = InitialPosition;
    FVector NewScale = InitialScale;

    switch(Type)
    {
        case EControlPanelWidgetType::ROTATION_BUTTON:
            if(bIsEffectActive)
            {
                FRotator NewRotator = ConnectedActor->GetActorRotation();

                NewRotator.Yaw += 50.f * EffectScale * DeltaTime;

                ConnectedActor->SetActorRelativeRotation(NewRotator);
            }
            break;   

        case EControlPanelWidgetType::TRANSLATION_BUTTON:
            if(bIsEffectActive)
            {
                NewVector += InitialDirection * 100.f * EffectScale;                
            }

            NewVector = FMath::Lerp(ConnectedActor->GetActorLocation(), NewVector, DeltaTime * 5.f * EffectScale);

            ConnectedActor->SetActorLocation(NewVector);
            break;     
        
        case EControlPanelWidgetType::SCALE_BUTTON:
            if(bIsEffectActive)
            {
                NewScale *= EffectScale;                
            }

            NewScale = FMath::Lerp(ConnectedActor->GetActorRelativeScale3D(), NewScale, DeltaTime * 5.f * EffectScale);

            ConnectedActor->SetActorRelativeScale3D(NewScale);
            break;
    } 
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
// Effect functions
// --------------------
void UControlPanelWidget::SetEffectActive(bool bNewState)
{
    bIsEffectActive = bNewState;

    switch(Type)
    {
        case EControlPanelWidgetType::GRAVITY_BUTTON:
            SetGravity(bIsEffectActive);
            break;

        case EControlPanelWidgetType::COLLISION_BUTTON:
            SetCollision(bIsEffectActive);
            break;
    }
}

bool UControlPanelWidget::IsEffectActive()
{
    switch(Type)
    {
        case EControlPanelWidgetType::GRAVITY_BUTTON:
            bIsEffectActive = GetGravity();
            break;

        case EControlPanelWidgetType::COLLISION_BUTTON:
            bIsEffectActive = GetCollision();
            break;
    }

    return bIsEffectActive;
}

bool UControlPanelWidget::GetGravity()
{
    UMeshComponent* MeshComponent = GetActorMeshComponent();
    
    if(!MeshComponent) { return false; }

    return MeshComponent->IsGravityEnabled();
}

void UControlPanelWidget::SetGravity(bool bIsEnabled)
{
    UMeshComponent* MeshComponent = GetActorMeshComponent();
    
    if(!MeshComponent) { return; }

    MeshComponent->SetEnableGravity(bIsEnabled);
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
