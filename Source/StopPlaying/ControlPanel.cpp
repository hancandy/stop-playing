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

void AControlPanel::SetConnectedActor(AEnvironmentActor* NewActor)
{
    if(!NewActor) { return; }

    if(ConnectedActor)
    {
        ConnectedActor->Toggle(false);
    }

    ConnectedActor = NewActor;
    TransformTarget = ConnectedActor->GetTransform();

    ConnectedActor->Toggle(true);

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
            SetConnectedActor(ActorConnector->ConnectedActor);
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
        Name = FText::FromString(ConnectedActor->Name);
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

     UMeshComponent* MeshComponent = ConnectedActor->FindComponentByClass<UMeshComponent>();
    
    if(!MeshComponent)
    {
            TArray<USceneComponent*> SceneComponents;
            
            ConnectedActor->GetRootComponent()->GetChildrenComponents(true, SceneComponents);
            
            for(USceneComponent* SceneComponent : SceneComponents)
            {
                UMeshComponent* ChildMeshComponent = Cast<UMeshComponent>(SceneComponent);
                
                if(ChildMeshComponent)
                {
                    MeshComponent = ChildMeshComponent;
                    break;
                }
            }
    }
    
    if(!MeshComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("%s has no UMeshComponent!"), *ConnectedActor->GetName());
        return false;
    }

    return MeshComponent->GetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody) == ECollisionResponse::ECR_Block;
}

void AControlPanel::SetCollision(bool bIsEnabled)
{
    if(!ConnectedActor) { return; }

    UMeshComponent* MeshComponent = ConnectedActor->FindComponentByClass<UMeshComponent>();
    
    if(!MeshComponent)
    {
            TArray<USceneComponent*> SceneComponents;
            
            ConnectedActor->GetRootComponent()->GetChildrenComponents(true, SceneComponents);
            
            for(USceneComponent* SceneComponent : SceneComponents)
            {
                UMeshComponent* ChildMeshComponent = Cast<UMeshComponent>(SceneComponent);
                
                if(ChildMeshComponent)
                {
                    MeshComponent = ChildMeshComponent;
                    break;
                }
            }
    }
    
    if(!MeshComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("%s has no UMeshComponent!"), *ConnectedActor->GetName());
        return;
    }

    if(bIsEnabled)
    {
        MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Block);
        MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
        MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
    }
    else
    {
        MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
        MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Ignore);
        MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
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
    if(!ConnectedActor) { return; }
    
    TranslationTimer = 1.f;
   
    FVector InitialLocation = ConnectedActor->InitialTransform.GetLocation(); 
    FVector NewLocation = InitialLocation;
    
    FVector ForwardDirection = FVector::ForwardVector;
    FVector RightDirection = FVector::RightVector;
    FVector UpDirection = FVector::UpVector;

    if(bIsEnabled)
    {
        if(!ConnectedActor->ConstrainTranslation.X)
        {
            NewLocation += ForwardDirection * EffectScale * 100.f;
        }

        if(!ConnectedActor->ConstrainTranslation.Y)
        {
            NewLocation += RightDirection * EffectScale * 100.f;
        }
        
        if(!ConnectedActor->ConstrainTranslation.Z)
        {
            NewLocation += UpDirection * EffectScale * 100.f;
        }
    }
    
    TransformTarget.SetLocation(NewLocation);
}

bool AControlPanel::GetTranslation()
{
    if(!ConnectedActor) { return false; }

    return TransformTarget.GetLocation() != ConnectedActor->InitialTransform.GetLocation();
}

void AControlPanel::SetRotation(bool bIsEnabled, float EffectScale)
{
    if(!ConnectedActor) { return; }
    
    RotationTimer = 1.f;
   
    FRotator InitialRotation = ConnectedActor->InitialTransform.Rotator(); 
    FRotator NewRotation = InitialRotation;

    if(bIsEnabled)
    {
        if(!ConnectedActor->ConstrainRotation.X)
        {
            NewRotation.Roll += EffectScale;
        }

        if(!ConnectedActor->ConstrainRotation.Y)
        {
            NewRotation.Pitch += EffectScale;
        }
        
        if(!ConnectedActor->ConstrainRotation.Z)
        {
            NewRotation.Yaw += EffectScale;
        }
    }
    
    TransformTarget.SetRotation(NewRotation.Quaternion());
}

bool AControlPanel::GetRotation()
{
    if(!ConnectedActor) { return false; }

    return TransformTarget.GetRotation() != ConnectedActor->InitialTransform.GetRotation();
}

void AControlPanel::SetScale(bool bIsEnabled, float EffectScale)
{
    if(!ConnectedActor) { return; }

    ScaleTimer = 1.f;
   
    FVector InitialScale = ConnectedActor->InitialTransform.GetScale3D(); 
    FVector NewScale = InitialScale;
    float NewMassScale = 1.f;

    if(bIsEnabled)
    {
        if(!ConnectedActor->ConstrainScale.X)
        {
            NewScale.X *= EffectScale;
            NewMassScale += EffectScale / 3;
        }

        if(!ConnectedActor->ConstrainScale.Y)
        {
            NewScale.Y *= EffectScale;
            NewMassScale += EffectScale / 3;
        }
        
        if(!ConnectedActor->ConstrainScale.Z)
        {
            NewScale.Z *= EffectScale;
            NewMassScale += EffectScale / 3;
        }
    }
    
    TransformTarget.SetScale3D(NewScale);

    UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(ConnectedActor->GetRootComponent());

    if(!PrimitiveComponent) { return; }

    PrimitiveComponent->SetMassScale(NAME_None, NewMassScale);
}

bool AControlPanel::GetScale()
{
    if(!ConnectedActor) { return false; }

    return TransformTarget.GetScale3D() != ConnectedActor->InitialTransform.GetScale3D();
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
