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

    if(!ConnectedActor) { return; }

/*
    FVector NewVector = InitialPosition;
    FVector NewScale = InitialScale;

    switch(Type)
    {
        case EControlPanelButtonType::ROTATION:
            if(bIsEffectActive)
            {
                FRotator NewRotator = ConnectedActor->GetActorRotation();

                NewRotator.Yaw += 50.f * EffectScale * DeltaTime;

                ConnectedActor->SetActorRelativeRotation(NewRotator);
            }
            break;   

        case EControlPanelButtonType::TRANSLATION:
            if(bIsEffectActive)
            {
                NewVector += InitialDirection * 100.f * EffectScale;                
            }

            NewVector = FMath::Lerp(ConnectedActor->GetActorLocation(), NewVector, DeltaTime * 5.f * EffectScale);

            ConnectedActor->SetActorLocation(NewVector);
            break;     
        
        case EControlPanelButtonType::SCALE:
            if(bIsEffectActive)
            {
                NewScale *= EffectScale;                
            }

            NewScale = FMath::Lerp(ConnectedActor->GetActorRelativeScale3D(), NewScale, DeltaTime * 5.f * EffectScale);

            ConnectedActor->SetActorRelativeScale3D(NewScale);
            break;
    }
   */ 
}

void AControlPanel::SetConnectedActor(AActor* NewActor)
{
    ConnectedActor = NewActor;

    UpdateAllWidgets();
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

    // Loop all actor pickers
    //GetComponents(ActorPickers);

    // Loop all buttons
    GetComponents(Buttons);

    for(UControlPanelButton* Button : Buttons)
    {
        if(!Button) { return; }

        Button->Init();
    }
}

void AControlPanel::UpdateAllWidgets()
{
    //for(UControlPanelActorPicker* ActorPicker : ActorPickers)
    //{
    //    if(!ActorPicker) { continue; }

    //    ActorPicker->OnUpdateWidgetAppearance(ActorPicker->ConnectedActor == ConnectedActor);
    //}

    for(UControlPanelButton* Button : Buttons)
    {
        if(!Button) { continue; }

        Button->OnUpdateWidgetAppearance(IsEffectActive(Button->EffectType, Button->EffectScale));
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

UMeshComponent* AControlPanel::GetMeshComponent()
{
    if(!ConnectedActor) { return nullptr; }

    return ConnectedActor->FindComponentByClass<UMeshComponent>();
}

// --------------------
// Effect functions
// --------------------
void AControlPanel::SetEffectActive(EControlPanelEffectType Type, float EffectScale, bool bIsEffectActive)
{
    switch(Type)
    {
        case EControlPanelEffectType::GRAVITY:
            SetGravity(bIsEffectActive);
            break;

        case EControlPanelEffectType::COLLISION:
            SetCollision(bIsEffectActive);
            break;
        
        case EControlPanelEffectType::TIME:
            SetTime(bIsEffectActive, EffectScale);
            break;
    }
}

bool AControlPanel::IsEffectActive(EControlPanelEffectType Type, float EffectScale)
{
    bool bIsEffectActive = false;

    switch(Type)
    {
        case EControlPanelEffectType::GRAVITY:
            bIsEffectActive = GetGravity();
            break;

        case EControlPanelEffectType::COLLISION:
            bIsEffectActive = GetCollision();
            break;
        
        case EControlPanelEffectType::TIME:
            bIsEffectActive = GetTime(EffectScale);
            break;
    }

    return bIsEffectActive;
}

bool AControlPanel::GetGravity()
{
    UMeshComponent* MeshComponent = GetMeshComponent();

    if(!MeshComponent) { return false; }

    return MeshComponent->IsGravityEnabled();
}

void AControlPanel::SetGravity(bool bIsEnabled)
{
    UMeshComponent* MeshComponent = GetMeshComponent();

    if(!MeshComponent) { return; }

    MeshComponent->SetEnableGravity(bIsEnabled);
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
}

bool AControlPanel::GetTime(float EffectScale)
{
    return GetWorld()->GetWorldSettings()->GetEffectiveTimeDilation() == EffectScale;
}

void AControlPanel::SetTime(bool bIsEnabled, float EffectScale)
{
    float NewTimeDilation = 1.f;

    if(bIsEnabled)
    {
        NewTimeDilation = EffectScale;
    }

    GetWorld()->GetWorldSettings()->SetTimeDilation(NewTimeDilation);

    GetWorld()->GetFirstPlayerController()->CustomTimeDilation = 1.f / NewTimeDilation;
    UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->CustomTimeDilation = 1.f / NewTimeDilation;
}
