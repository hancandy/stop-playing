#include "StopPlaying.h"
#include "ControlPanel.h"

AControlPanel::AControlPanel()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AControlPanel::BeginPlay()
{
	Super::BeginPlay();

    // Fetch the player actor if necessary
    if(bConnectedActorIsPlayer)
    {
        ConnectedActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    }

    AssignAllWidgets();
    InitAllWidgets();

    SetTitle();
}

void AControlPanel::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void AControlPanel::InitAllWidgets()
{
    SetGravity(GetGravity());
    SetCollision(GetCollision());
}

void AControlPanel::AssignAllWidgets()
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
    TArray<UControlPanelWidget*> Widgets;

    GetComponents(Widgets);

    for(UControlPanelWidget* Widget : Widgets)
    {
        InitWidget(Widget);
    }
}

void AControlPanel::InitWidget(UControlPanelWidget* Widget)
{
    if(!Widget) { return; }
    
    AActor* ChildActor = Widget->GetChildActor();

    if(!ChildActor)

    switch(Widget->Type)
    {
        case EControlPanelWidgetType::GRAVITY_BUTTON:
            GravityButton = Cast<AInteractiveActor>(ChildActor);
            
            if(GravityButton)
            {   
                UE_LOG(LogTemp, Warning, TEXT("%s found \"GravityButton\" %s"), *GetName(), *GravityButton->GetName());
        
                GravityButton->OnInteraction.AddDynamic(this, &AControlPanel::OnClickGravityButton);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("%s has a \"GravityButton\", but it's not an AInteractiveActor class"), *GetName());
            }
            break;
        
        case EControlPanelWidgetType::COLLISION_BUTTON:
            CollisionButton = Cast<AInteractiveActor>(ChildActor);

            if(CollisionButton)
            {   
                UE_LOG(LogTemp, Warning, TEXT("%s found \"CollisionButton\" %s"), *GetName(), *CollisionButton->GetName());
                
                CollisionButton->OnInteraction.AddDynamic(this, &AControlPanel::OnClickCollisionButton);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("%s has a \"CollisionButton\", but it's not an AInteractiveActor class"), *GetName());
            }
            break;
    }
}

bool AControlPanel::HasConnectedActor()
{
    if(!ConnectedActor)
    {
        UE_LOG(LogTemp, Error, TEXT("%s has no connected AActor"), *GetName());
        return false;
    }

    return true;
}

UPrimitiveComponent* AControlPanel::GetActorPrimitiveComponent()
{
    if(!HasConnectedActor()) { return nullptr; }

    UPrimitiveComponent* PrimitiveComponent = ConnectedActor->FindComponentByClass<UPrimitiveComponent>();

    if(!PrimitiveComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("%s has no UPrimitiveComponent"), *ConnectedActor->GetName());
    }

    return PrimitiveComponent;
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

void AControlPanel::SetWidgetText(AInteractiveActor* Widget, FString NewText)
{
    if(!Widget) { return; }

    UTextRenderComponent* TextRenderComponent = Widget->FindComponentByClass<UTextRenderComponent>();

    if(TextRenderComponent)
    {
        TextRenderComponent->SetText(FText::FromString(NewText));
    }
}

// ------------------
// Events
// ------------------
void AControlPanel::OnClickGravityButton(APawn* InteractingPawn)
{
    if(!InteractingPawn) { return; }
    if(!GravityButton) { return; }

    UE_LOG(LogTemp, Warning, TEXT("%s pressed the gravity button on panel %s"), *InteractingPawn->GetName(), *GetName());

    SetGravity(!GetGravity());
}

void AControlPanel::OnClickCollisionButton(APawn* InteractingPawn)
{
    if(!InteractingPawn) { return; }
    if(!CollisionButton) { return; }

    UE_LOG(LogTemp, Warning, TEXT("%s pressed the collision button on panel %s"), *InteractingPawn->GetName(), *GetName());

    SetCollision(!GetCollision());
}

// ------------------
// Getters/setters
// ------------------
bool AControlPanel::GetGravity()
{
    UPrimitiveComponent* PrimitiveComponent = GetActorPrimitiveComponent();
    
    if(!PrimitiveComponent) { return false; }

    return PrimitiveComponent->IsGravityEnabled();
}

void AControlPanel::SetGravity(bool bIsEnabled)
{
    UPrimitiveComponent* PrimitiveComponent = GetActorPrimitiveComponent();
    
    if(!PrimitiveComponent) { return; }

    PrimitiveComponent->SetEnableGravity(bIsEnabled);

    FString Label = "Gravity ";

    if(bIsEnabled)
    {
        Label+= "ON";
    }
    else
    {
        Label+= "OFF";
    }

    SetWidgetText(GravityButton, Label);
}

bool AControlPanel::GetCollision()
{
    if(!ConnectedActor) { return false; }

    return ConnectedActor->GetActorEnableCollision();
}

void AControlPanel::SetCollision(bool bIsEnabled)
{
    if(!ConnectedActor) { return; }

    ConnectedActor->SetActorEnableCollision(bIsEnabled);

    FString Label = "Collision ";

    if(bIsEnabled)
    {
        Label+= "ON";
    }
    else
    {
        Label+= "OFF";
    }

    SetWidgetText(CollisionButton, Label);
}

