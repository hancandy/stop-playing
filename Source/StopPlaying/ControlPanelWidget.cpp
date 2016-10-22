#include "StopPlaying.h"
#include "ControlPanelWidget.h"

UControlPanelWidget::UControlPanelWidget()
{
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}

void UControlPanelWidget::Init()
{
    // Assign relevant actors
    ChildActor = GetInteractiveActor();
            
    if(!ChildActor) {
        UE_LOG(LogTemp, Error, TEXT("%s has no child actor"), *GetName());
        return;
    }
    
    // Add interaction event
    ChildActor->OnInteraction.AddDynamic(this, &UControlPanelWidget::OnInteract);
}

AInteractiveActor* UControlPanelWidget::GetInteractiveActor()
{
    return Cast<AInteractiveActor>(GetChildActor());
}

void UControlPanelWidget::SetLabel(FString NewString)
{
    if(!GetInteractiveActor()) { return; }

    UTextRenderComponent* TextRenderComponent = GetInteractiveActor()->FindComponentByClass<UTextRenderComponent>();

    if(!TextRenderComponent) { return; }

    TextRenderComponent->SetText(FText::FromString(NewString));
}

void UControlPanelWidget::OnInteract(APawn* InteractingPawn)
{
}
