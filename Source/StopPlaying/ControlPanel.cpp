#include "StopPlaying.h"
#include "ControlPanel.h"
#include "DefaultPlayer.h"

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

    if(UpdateWidgetsTimer <= 0.f)
    {
        UpdateAllWidgets();
        UpdateWidgetsTimer = 0.5f;
    }
    else
    {
        UpdateWidgetsTimer -= DeltaTime;
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

    FVector NewScale = FMath::Lerp(ConnectedActor->GetActorScale3D(), TransformTarget.GetScale3D(), 1.f - ScaleTimer);

    ConnectedActor->SetActorScale3D(NewScale);  

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
    if(!NewActor) { return; }

    AInteractiveActor* ConnectedInteractiveActor = nullptr;

    if(ConnectedActor)
    {
        ConnectedInteractiveActor = Cast<AInteractiveActor>(ConnectedActor);
        
        if(ConnectedInteractiveActor)
        {
            ConnectedInteractiveActor->Toggle(false);
        }
    }

    ConnectedActor = NewActor;
    InitialTransform = NewTransform;
    TransformTarget = ConnectedActor->GetTransform();

    ConnectedInteractiveActor = Cast<AInteractiveActor>(ConnectedActor);
    
    if(ConnectedInteractiveActor)
    {
        ConnectedInteractiveActor->Toggle(true);
    }

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

        if(!ConnectedActor && ActorConnector->ConnectedActor)
        {
            SetConnectedActor(ActorConnector->ConnectedActor, ActorConnector->ConnectedActor->GetTransform());
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

        ActorConnector->UpdateWidgetState(ActorConnector->ConnectedActor == ConnectedActor);
    }

    for(UControlPanelButton* Button : Buttons)
    {
        if(!Button) { continue; }

        bool bIsEffectActive = IsEffectActive(Button->EffectType);
        
        Button->SetAppropriateLabel();
        Button->UpdateWidgetState(bIsEffectActive);
    }
}

void AControlPanel::SetTitle()
{
    FText Name;

    if(ConnectedActor)
    {
        Name = FText::FromString(ConnectedActor->GetName());
    }
    else
    {
        Name = FText::FromString("(NONE)");
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
        
        case EControlPanelEffectType::WORLD_TIME:
            SetWorldTime(bIsEffectActive, EffectScale);
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
        
        case EControlPanelEffectType::WORLD_GRAVITY:
            SetWorldGravity(bIsEffectActive, EffectScale);
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
        
        case EControlPanelEffectType::WORLD_TIME:
            bIsEffectActive = GetWorldTime();
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
        
        case EControlPanelEffectType::WORLD_GRAVITY:
            bIsEffectActive = GetWorldGravity();
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

    UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(ConnectedActor->GetRootComponent());
    
    if(!PrimitiveComponent) {
        UE_LOG(LogTemp, Error, TEXT("%s has no UPrimitiveComponent!"), *ConnectedActor->GetName());
        return false;
    }

    return PrimitiveComponent->GetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody) == ECollisionResponse::ECR_Block;
}

void AControlPanel::SetCollision(bool bIsEnabled)
{
    if(!ConnectedActor) { return; }

    UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(ConnectedActor->GetRootComponent());
    
    if(!PrimitiveComponent) {
        UE_LOG(LogTemp, Error, TEXT("%s has no UPrimitiveComponent!"), *ConnectedActor->GetName());
        return;
    }

    if(bIsEnabled)
    {
        PrimitiveComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Block);
        PrimitiveComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
    }
    else
    {
        PrimitiveComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
        PrimitiveComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Ignore);
    }
}

bool AControlPanel::GetWorldTime()
{
    return GetWorld()->GetWorldSettings()->GetEffectiveTimeDilation() != 1.f;
}

void AControlPanel::SetWorldTime(bool bIsEnabled, float EffectScale)
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

bool AControlPanel::GetWorldGravity()
{
    ADefaultPlayer* DefaultPlayer = Cast<ADefaultPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

    if(DefaultPlayer)
    {
        return DefaultPlayer->GetGravityScale() != 1.f;
    }

    return false;
}

void AControlPanel::SetWorldGravity(bool bIsEnabled, float EffectScale)
{
    ADefaultPlayer* DefaultPlayer = Cast<ADefaultPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

    float NewGravityScale = 1.f;

    if(bIsEnabled)
    {
        NewGravityScale = EffectScale;
    }

    if(DefaultPlayer)
    {
        DefaultPlayer->SetGravityScale(NewGravityScale);
    }
}
