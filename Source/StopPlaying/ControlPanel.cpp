#include "StopPlaying.h"
#include "ControlPanel.h"

AControlPanel::AControlPanel()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AControlPanel::BeginPlay()
{
	Super::BeginPlay();

    InitAllWidgets();
    
    SetTitle();

    UpdateAllWidgets();
}

void AControlPanel::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

    if(!ConnectedActor) { return; }

    if(TranslationTimer > 0.f)
    {
        TickTranslationTimer(DeltaTime);
    }

    if(RotationTimer > 0.f)
    {
        TickRotationTimer(DeltaTime);
    }
    
    if(ScaleTimer > 0.f)
    {
        TickScaleTimer(DeltaTime);
    }
}

void AControlPanel::TickTranslationTimer(float DeltaTime)
{
    if(!ConnectedActor) { return; } 

    FVector NewLocation = FMath::Lerp(ConnectedActor->GetActorLocation(), TransformTarget.GetLocation(), 1.f - TranslationTimer);

    ConnectedActor->SetActorLocation(NewLocation);  

    TranslationTimer -= DeltaTime;
}

void AControlPanel::TickScaleTimer(float DeltaTime)
{
    if(!ConnectedActor) { return; } 

    FVector NewScale = FMath::Lerp(ConnectedActor->GetActorRelativeScale3D(), TransformTarget.GetScale3D(), 1.f - ScaleTimer);

    ConnectedActor->SetActorRelativeScale3D(NewScale);  

    ScaleTimer -= DeltaTime;
}

void AControlPanel::TickRotationTimer(float DeltaTime)
{
    if(!ConnectedActor) { return; } 

    FRotator NewRotation = FMath::Lerp(ConnectedActor->GetActorRotation(), TransformTarget.Rotator(), 1.f - RotationTimer);

    ConnectedActor->SetActorRotation(NewRotation);  

    RotationTimer -= DeltaTime;
}

void AControlPanel::SetConnectedActor(AActor* NewActor, FTransform NewTransform)
{
    ConnectedActor = NewActor;
    InitialTransform = NewTransform;
    TransformTarget = ConnectedActor->GetTransform();

    UpdateAllWidgets();

    SetTitle();
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
    GetComponents(ActorConnectors);
    
    for(UControlPanelActorConnector* ActorConnector : ActorConnectors)
    {
        if(!ActorConnector) { return; }

        ActorConnector->Init();

        if(!ConnectedActor)
        {
            ConnectedActor = ActorConnector->ConnectedActor;
        }
    }

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
    for(UControlPanelActorConnector* ActorConnector : ActorConnectors)
    {
        if(!ActorConnector) { continue; }

        ActorConnector->OnUpdateWidgetAppearance(ActorConnector->ConnectedActor == ConnectedActor);
    }

    for(UControlPanelButton* Button : Buttons)
    {
        if(!Button) { continue; }

        bool bIsEffectActive = IsEffectActive(Button->EffectType);
        
        Button->SetAppropriateLabel();
        Button->OnUpdateWidgetAppearance(bIsEffectActive);
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

        case EControlPanelEffectType::TRANSLATION:
            SetTranslation(bIsEffectActive, EffectScale);
            break;
        
        case EControlPanelEffectType::ROTATION:
            SetRotation(bIsEffectActive, EffectScale);
            break;
        
        case EControlPanelEffectType::SCALE:
            SetScale(bIsEffectActive, EffectScale);
            break;
    }

    UpdateAllWidgets();
}

bool AControlPanel::IsEffectActive(EControlPanelEffectType Type)
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
            bIsEffectActive = GetTime();
            break;

        case EControlPanelEffectType::TRANSLATION:
            bIsEffectActive = GetTranslation();
            break;
        
        case EControlPanelEffectType::ROTATION:
            bIsEffectActive = GetRotation();
            break;
        
        case EControlPanelEffectType::SCALE:
            bIsEffectActive = GetScale();
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

bool AControlPanel::GetTime()
{
    return GetWorld()->GetWorldSettings()->GetEffectiveTimeDilation() != 1.f;
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

void AControlPanel::SetTranslation(bool bIsEnabled, float EffectScale)
{
    TranslationTimer = 1.f;
   
    FVector InitialLocation = InitialTransform.GetLocation(); 
    FVector InitialDirection = InitialTransform.GetRotation().GetForwardVector();

    if(bIsEnabled)
    {
        InitialLocation += InitialDirection * EffectScale * 100.f;
    }
    
    TransformTarget.SetLocation(InitialLocation);
}

bool AControlPanel::GetTranslation()
{
    if(!ConnectedActor) { return false; }

    return TransformTarget.GetLocation() != InitialTransform.GetLocation();
}

void AControlPanel::SetRotation(bool bIsEnabled, float EffectScale)
{
    RotationTimer = 1.f;
   
    FRotator InitialRotation = InitialTransform.Rotator(); 

    if(bIsEnabled)
    {
        InitialRotation.Yaw += EffectScale;
    }
    
    TransformTarget.SetRotation(InitialRotation.Quaternion());
}

bool AControlPanel::GetRotation()
{
    if(!ConnectedActor) { return false; }

    return TransformTarget.GetRotation() != InitialTransform.GetRotation();
}

void AControlPanel::SetScale(bool bIsEnabled, float EffectScale)
{
    ScaleTimer = 1.f;
   
    FVector InitialScale = InitialTransform.GetScale3D(); 

    if(bIsEnabled)
    {
        InitialScale *= EffectScale;
    }
    
    TransformTarget.SetScale3D(InitialScale);
}

bool AControlPanel::GetScale()
{
    if(!ConnectedActor) { return false; }

    return TransformTarget.GetScale3D() != InitialTransform.GetScale3D();
}
