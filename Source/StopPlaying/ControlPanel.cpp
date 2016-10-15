#include "StopPlaying.h"
#include "ControlPanel.h"

AControlPanel::AControlPanel()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AControlPanel::BeginPlay()
{
	Super::BeginPlay();

    if(!ConnectedActor) {
        UE_LOG(LogTemp, Error, TEXT("%s has no connected actor"), *GetName());
        return;
    }
    
    InitAllWidgets();

    SetTitle();
}

void AControlPanel::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

    for(UControlPanelWidget* Widget : Widgets)
    {
        if(!Widget) { continue; }

        Widget->TickWidget(DeltaTime);
    }
}

void AControlPanel::InitAllWidgets()
{
    // Loop all text render components
    TArray<UTextRenderComponent*> TextRenderComponents;

    GetComponents(TextRenderComponents);

    for(UTextRenderComponent* TextRenderComponent : TextRenderComponents)
    {
        if(!TextRenderComponent) { continue; }

        if(TextRenderComponent->GetName() == "Title")
        {
            TitleComponent = TextRenderComponent;
        }
    }

    // Loop all widgets
    GetComponents(Widgets);

    for(UControlPanelWidget* Widget : Widgets)
    {
        if(!Widget) { continue; }

        Widget->Init(ConnectedActor);
    }
}

void AControlPanel::SetTitle()
{
    FText Name;

    if(ConnectedActor)
    {
        Name = FText::FromString("Object: " + ConnectedActor->GetName());
    }
    else
    {
        Name = FText::FromString("Object: (NONE)");
    }

    if(!TitleComponent) { return; }

    TitleComponent->SetText(Name);
}
